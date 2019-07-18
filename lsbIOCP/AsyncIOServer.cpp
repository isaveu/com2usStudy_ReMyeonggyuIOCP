#include "AsyncIOServer.h"

size_t AsyncIOServer::IO_MIN_SIZE = 1024;

AsyncIOServer::AsyncIOServer(
	IServerReceiver* pReceiver,
	DWORD ioMaxSize,
	DWORD threadNumber,
	DWORD sessionNumber,
	std::string name)
	: m_pReceiver(pReceiver)
	, m_IOMaxSize(ioMaxSize)
	, m_ThreadNum(threadNumber)
	, m_ServerName(name)
	, m_IOCPHandle(INVALID_HANDLE_VALUE)
	, m_Log(Log::GetInstance())
{
	Log::GetInstance()->Init(LOG_LEVEL::DEBUG, name);

	if (name == "") 
	{
		m_ServerName = utils::Format("AsyncIOServer_%d", ::GetCurrentThreadId()); 
	}

	ThrowErrorIf(pReceiver == nullptr, WSAEINVAL, "Receiver nullptr");
	ThrowErrorIf(threadNumber < 1, WSAEINVAL, "Thread number must over than 0");
	ThrowErrorIf(IO_MIN_SIZE > ioMaxSize, WSAEMSGSIZE, "ioMaxSize is too small than IO_MIN_SIZE");
	ThrowErrorIf(SessionManager::SESSION_MAX_NUMBER < sessionNumber, WSAENOBUFS, "Session number exceeds max value");

	m_IOCPHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, static_cast<ULONG_PTR>(0), threadNumber);

	ThrowLastErrorIf(m_IOCPHandle == INVALID_HANDLE_VALUE, "Fail create IOCP");
	
	m_pSessionManager = new SessionManager(sessionNumber, ioMaxSize, this);

	for (DWORD i = 0; i < threadNumber; i++)
	{
		m_Workers.push_back(std::make_shared<Worker>(pReceiver, m_IOCPHandle, m_pSessionManager, this));
	}

	m_Log->Write("Server Initialized succesfully");
};

AsyncIOServer::~AsyncIOServer()
{
	if (INVALID_HANDLE_VALUE != m_IOCPHandle)
	{
		CloseHandle(m_IOCPHandle);
	}
}

void AsyncIOServer::Start()
{
	for (auto& worker : m_Workers)
	{
		worker->Start();
	}
}

void AsyncIOServer::Stop()
{
	for (auto& worker : m_Workers)
	{
		worker->Stop();
	}
}

void AsyncIOServer::Join()
{
	for (auto& worker : m_Workers)
	{
		worker->Join();
	}

}

DWORD AsyncIOServer::RegisterSession(SOCKET clientSocket)
{
	if (clientSocket == INVALID_SOCKET) return WSAENOTSOCK;
	
	size_t sessionId;
	if (m_pSessionManager->retrieveId(sessionId) == false) return WSAENOBUFS;

	auto session = m_pSessionManager->GetSessionPtr(sessionId);
	session->OpenSocket(clientSocket);

	m_Log->Write(utils::Format("[session #%u] Socket added to session", session->GetSessionDescRef().id));

	auto res = ::CreateIoCompletionPort(
		reinterpret_cast<HANDLE>(clientSocket),
		m_IOCPHandle,
		sessionId,
		0);

	if (res == NULL) ReleaseSession(sessionId, ::GetLastError());

	m_pReceiver->NotifyClientConnected(session->GetSessionDescRef());

	auto error = PostRecv(session);
	if (error != NULL) ReleaseSession(sessionId, error);

	return 0;
}

DWORD AsyncIOServer::ReleaseSession(size_t sessionId, DWORD error)
{
	auto session = m_pSessionManager->GetSessionPtr(sessionId);
	if (session->IsPossibleClose() == false) return 0;

	if (error == NULL)
		m_Log->Write(utils::Format("[session #%u] Release Sokcet", sessionId));
	else
		m_Log->Write(utils::Format("[session #%u] Release Sokcet by Error #%d", sessionId, error));

	session->CloseSocket();
	m_pSessionManager->returnId(sessionId);

	return 0;
}

DWORD AsyncIOServer::PostRecv(SESSION* session)
{
	if (session == nullptr) return WSAEINVAL;

	auto lpOverlapped = session->GetOverlapped(OP_TYPE::RECV);
	auto& wsabuf = lpOverlapped->wsabuf;

	// session->enterIO();

	// TODO: 없이 한 번 해보기
	lpOverlapped->Init();

	lpOverlapped->type = OP_TYPE::RECV;
	wsabuf.len = static_cast<ULONG>(m_IOMaxSize);

	DWORD bufferCount = 1;
	DWORD flags = 0;
	DWORD nbytes = 0;
	auto res = WSARecv(
		session->GetSocket(),
		&wsabuf,
		bufferCount,
		&nbytes,
		&flags,
		&lpOverlapped->overlapped,
		NULL);

	if (res == SOCKET_ERROR)
	{
		auto error = WSAGetLastError();
		if (error != WSA_IO_PENDING) return error;
	}

	return 0;
}

DWORD AsyncIOServer::PostSend(SESSION* session, size_t length, char* data)
{
	if (length <= 0 || length > m_IOMaxSize) return WSAEMSGSIZE;
	if (length == 0 || data == nullptr) return WSAEINVAL;
	if (session == nullptr) return WSAEINVAL;

	// TODO: 우아한 lock-free 고민
	if (session->IsOpened() == false) return WSAEINVAL;
	
	auto lpOverlapped = session->GetOverlapped(OP_TYPE::SEND);
	auto& wsabuf = lpOverlapped->wsabuf;

	// session->enterIO();

	// TODO: 없이 한 번 해보기
	lpOverlapped->Init();

	lpOverlapped->type = OP_TYPE::SEND;
	wsabuf.len = static_cast<ULONG>(length);
	CopyMemory(wsabuf.buf, data, length);

	DWORD bufferCount = 1;
	DWORD flags = 0;
	DWORD nbytes = 0;
	auto res = WSASend(
		session->GetSocket(),
		&wsabuf,
		bufferCount,
		&nbytes,
		flags,
		reinterpret_cast<LPWSAOVERLAPPED>(lpOverlapped),
		NULL);

	if (res == SOCKET_ERROR)
	{
		auto error = WSAGetLastError();
		if (error != WSA_IO_PENDING && error != ERROR_SUCCESS) return error;
	}

	return 0;
}
