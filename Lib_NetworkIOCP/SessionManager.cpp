#include "SessionManager.h"

// Constructor of SessionManager
// Create session object pool ( [sessionNum] sessions )
// Set unique id of each session and push id to concurrent_queue (session id queue)
SessionManager::SessionManager(const int sessionNum, const PacketBufferConfig pktBufferConfig, Log* const pLog)
	: m_SessionNumber(sessionNum), m_IOBufMaxSize(pktBufferConfig.bufferSize), m_pLogger(pLog)
{

	m_ConnectedSessionNumber.store(0);
	int sessionId = 0;
	m_SessionPool.assign(sessionNum, nullptr);
	for (auto& session : m_SessionPool)
	{
		session = new SESSION(pktBufferConfig);
		session->SetSessionId(sessionId);
		m_SessionIdPool.push(sessionId++);
	}

	pLog->Write(LV::DEBUG, "Create %d Sessions succesfully", sessionNum);
}

SessionManager::~SessionManager()
{
	m_SessionIdPool.clear();
	for (auto& session : m_SessionPool)
	{
		delete session;
	}
}

// Retrieve available session id and count up total connected session number
bool SessionManager::retrieveId(int& _out_sessionId)
{
	m_ConnectedSessionNumber++;
	return m_SessionIdPool.try_pop(_out_sessionId);
}

// Return available session id and count up total connected session number
void SessionManager::returnId(int sessionId)
{
	m_ConnectedSessionNumber--;
	m_SessionIdPool.push(sessionId);
}

// SESSION has atomic member values, so they can not use &(ref) constructor
// https://stackoverflow.com/questions/14182258/c11-write-move-constructor-with-atomicbool-member
// Get session of session id
SESSION* SessionManager::GetSessionPtr(int sessionId)
{
	return m_SessionPool.at(sessionId);
}

// Post WSARecv
NET_ERROR_CODE SessionManager::PostRecv(SESSION* pSession)
{
	if (pSession == nullptr)
	{
		return NET_ERROR_CODE::INVALID_SESSION;
	}

	// TODO: consider CAS structure by checking open flag
	{
		if (pSession->IsOpened() == false)
		{
			return NET_ERROR_CODE::SESSION_NOT_OPENED;
		};

		auto lpOverlapped = pSession->GetOverlapped(OP_TYPE::RECV);

		WSABUF wsabuf;
		wsabuf.buf = lpOverlapped->bufferMngr.WriteCurrent();
		wsabuf.len = lpOverlapped->bufferMngr.MaxWriteLegnth();

		// session->enterIO();

		// TODO: really needed?
		// lpOverlapped->Init();

		DWORD bufferCount = 1;
		DWORD flags = 0;
		DWORD nbytes = 0;
		auto res = WSARecv(
			pSession->GetSocket(),
			&wsabuf,
			bufferCount,
			&nbytes,
			&flags,
			&lpOverlapped->overlapped,
			NULL);

		if (res == SOCKET_ERROR && ::WSAGetLastError() != WSA_IO_PENDING)
		{
			return NET_ERROR_CODE::FAIL_WSA_RECV;
		}

		m_pLogger->Write(LV::DEBUG, "Posted WSARecv()");
	}

	return NET_ERROR_CODE::NONE;
}

// Post WSASend
NET_ERROR_CODE SessionManager::PostSend(SESSION* pSession, int length)
{
	// Check condition
	if (length <= 0 || length > m_IOBufMaxSize)
	{
		return NET_ERROR_CODE::SEND_DATA_EXCEEDED_BUFFER;
	};

	if (pSession == nullptr)
	{
		return NET_ERROR_CODE::INVALID_SESSION;
	};

	// TODO: consider CAS structure by checking open flag
	{
		if (pSession->IsOpened() == false)
		{
			return NET_ERROR_CODE::SESSION_NOT_OPENED;
		};

		auto lpOverlapped = pSession->GetOverlapped(OP_TYPE::SEND);

		WSABUF wsabuf;
		wsabuf.buf = lpOverlapped->bufferMngr.Read(length);
		wsabuf.len = static_cast<ULONG>(length);

		// pSession->enterIO();

		// TODO: really needed?
		// lpOverlapped->Init();

		DWORD bufferCount = 1;
		DWORD flags = 0;
		DWORD nbytes = 0;
		auto res = WSASend(
			pSession->GetSocket(),
			&wsabuf,
			bufferCount,
			&nbytes,
			flags,
			&lpOverlapped->overlapped,
			NULL);

		if (res == SOCKET_ERROR)
		{
			auto error = WSAGetLastError();
			if (error != WSA_IO_PENDING && error != ERROR_SUCCESS)
			{
				return NET_ERROR_CODE::FAIL_WSA_SEND;
			}
		}

		m_pLogger->Write(LV::DEBUG, "Posted WSARSend()");
	}

	return NET_ERROR_CODE::NONE;
}
