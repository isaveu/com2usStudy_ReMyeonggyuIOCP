#include "AsyncIONetwork.h"

AsyncIONetwork::AsyncIONetwork(INetworkReceiver* const pReceiver, NetworkConfig config)
	: m_pReceiver(pReceiver), m_IOCPHandle(INVALID_HANDLE_VALUE), m_Log(new Log())
{
	auto file = std::string("");
	m_Log->Init(LV::DISABLE, file);
	if (config.name == "")
	{
		m_ServerName = "AsyncIONetwork_" + utils::GetDate(); 
	}

	m_ThreadNum = config.threadNumber;
	m_ServerName = config.name;

	// Initialize Log file name, level
	// m_Log->Init(LOG_LEVEL::DEBUG, m_ServerName);

	// Config conditions
	ThrowErrorIf(pReceiver == nullptr, WSAEINVAL, "Receiver nullptr");
	ThrowErrorIf(config.bufferSize <= 0, WSAEMSGSIZE, "Buffer size is invalid");
	ThrowErrorIf(config.threadNumber < 1, WSAEINVAL, "Thread number must over than 0");

	// Create IOCP
	m_IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, static_cast<ULONG_PTR>(0), config.threadNumber);
	ThrowLastErrorIf(m_IOCPHandle == INVALID_HANDLE_VALUE, "Fail create IOCP");
	m_Log->Write(LV::DEBUG, "Create IOCP succesfully");

	// Initialize Session Manager (session pool, session id pool etc..)
	PacketBufferConfig pktBufferConfig = { config.bufferSize, config.headerSize, config.maxPacketSize };
	m_pSessionManager = new SessionManager(config.sessionNumber, pktBufferConfig, m_Log);
	m_Log->Write(LV::DEBUG, "Create Session manager succesfully");

	// Initialize Worker threads
	for (int i = 0; i < config.threadNumber; i++)
	{
		m_Workers.push_back(std::make_shared<Worker>(pReceiver, m_IOCPHandle, m_pSessionManager, this, m_Log));
	}
	m_Log->Write(LV::DEBUG, "Create %d threads succesfully", config.threadNumber);

	// Acceptor initialize
	// Apply acceptor to my server
	m_pAcceptor = new Acceptor(this, config.ip, config.port, m_Log);
};

AsyncIONetwork::~AsyncIONetwork()
{
	if (INVALID_HANDLE_VALUE != m_IOCPHandle)
	{
		CloseHandle(m_IOCPHandle);
	}
}

void AsyncIONetwork::Start()
{
	m_pAcceptor->Start();
	for (auto& worker : m_Workers)
	{
		worker->Start();
	}
}

void AsyncIONetwork::Stop()
{
	m_pAcceptor->Stop();
	for (auto& worker : m_Workers)
	{
		worker->Stop();
	}
}

void AsyncIONetwork::Join()
{
	m_pAcceptor->Join();
	for (auto& worker : m_Workers)
	{
		worker->Join();
	}
}

// Apply socket id to available session
// Warning : not yet opened socket flag
SESSION* AsyncIONetwork::LinkSocketToSession(const SOCKET clientSocket)
{
	// Get available session id from 'session id pool' (concurrent queue)
	int sessionId;
	if (m_pSessionManager->retrieveId(sessionId) == false)
	{
		m_Log->Write(LV::WARN, "Can not retrieve Session");
		return nullptr;
	}

	auto pSession = m_pSessionManager->GetSessionPtr(sessionId);
	// Initialize session info
	pSession->Clear();

	// Add this socket to current IOCP handle using session id as completion key
	auto res = ::CreateIoCompletionPort(
		reinterpret_cast<HANDLE>(clientSocket),
		m_IOCPHandle,
		sessionId,
		0);

	if (res != m_IOCPHandle)
	{
		// If fail to add client socket to iocp handle,
		// return current session id to 'session id pool'
		m_pSessionManager->returnId(sessionId);
		m_Log->Write(LV::WARN, "Can not link client socket to completion port");
		return nullptr;
	}

	// Link this session socket with client socket
	pSession->SetSocket(clientSocket);

	m_Log->Write(LV::INFO, "[session #%d] Socket added to session", sessionId);

	return pSession;
}

// Register already accepted client socket to server
// Then, notify to server the result and post receive
NET_ERROR_CODE AsyncIONetwork::RegisterClient(const SOCKET clientSocket)
{
	// Apply socket id to available session
	auto pSession = LinkSocketToSession(clientSocket);
	if (pSession == nullptr)
	{
		return NET_ERROR_CODE::FAIL_LINK_SOCKET_TO_SESSION;
	}

	// Notify to server that client socket successfully connected with session descriptor
	m_pReceiver->NotifyClientConnected(pSession->GetSessionId());

	// Socket is already accepted by accept() func. Set open flag
	if (pSession->Open() == false)
	{
		// If session is already opened, something wrong
		UnlinkSocketToSession(pSession->GetSessionId(), NET_ERROR_CODE::INVALID_SESSION);
	}

	// Post receive IOCP job
	auto ret = m_pSessionManager->PostRecv(pSession);
	if (ret != NET_ERROR_CODE::NONE) {
		m_Log->Write(LV::ERR, "[NET_ERROR #%d] PostRecv failed", ret);
		UnlinkSocketToSession(pSession->GetSessionId(), ret);
		return ret;
	}

	return NET_ERROR_CODE::NONE;
}

// Release current session id & close the socket if possible
void AsyncIONetwork::UnlinkSocketToSession(const int sessionId, const NET_ERROR_CODE error)
{
	auto pSession = m_pSessionManager->GetSessionPtr(sessionId);

	// Check this session is already closed, if not close socket
	if (pSession->Close())
	{
		pSession->SetSocket(INVALID_SOCKET);
		closesocket(pSession->GetSocket());

		m_Log->Write(LV::DEBUG, "Socket closed %d", pSession->GetSocket());
		m_pReceiver->NotifyClientDisconnected(pSession->GetSessionId());
	}

	// Return session id to session id pool
	m_pSessionManager->returnId(sessionId);

	if (error == NET_ERROR_CODE::NONE)
		m_Log->Write(LV::INFO, "[Session #%d] Release Sokcet", sessionId);
	else
		m_Log->Write(LV::ERR, "[NET_ERROR #%d] [Session #%d] Release Sokcet by Error", error, sessionId);
}