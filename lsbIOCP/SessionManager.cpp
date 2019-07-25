#include "SessionManager.h"

// Constructor of SessionManager
// Create session object pool ( [sessionNum] sessions )
// Set unique id of each session and push id to concurrent_queue (session id queue)
SessionManager::SessionManager(
	const INT sessionNum
	, SessionConfig sessionConfig
	, PacketBufferConfig pktBufferConfig
	, Log* const pLog)
	: m_SessionNumber(sessionNum), m_IOBufMaxSize(sessionConfig.ioBufMaxSize), m_Log(pLog)
{

	m_ConnectedSessionNumber.store(0);
	INT sessionId = 0;
	m_SessionPool.assign(sessionNum, nullptr);
	for (auto& session : m_SessionPool)
	{
		session = new SESSION(sessionConfig, pktBufferConfig);
		session->SetSessionId(sessionId);
		m_SessionIdPool.push(sessionId++);
	}

	pLog->Write(LV::DEBUG, "Create %d Sessions succesfully", sessionNum);
}

// Retrieve available session id and count up total connected session number
bool SessionManager::retrieveId(INT& _out_sessionId)
{
	m_ConnectedSessionNumber++;
	return m_SessionIdPool.try_pop(_out_sessionId);
}

// Return available session id and count up total connected session number
void SessionManager::returnId(INT sessionId)
{
	m_ConnectedSessionNumber--;
	m_SessionIdPool.push(sessionId);
}

// Get session descriptor of session id
SESSIONDESC& SessionManager::GetSessionDescRef(INT sessionId)
{
	return m_SessionPool.at(sessionId)->GetSessionDescRef();
}

// SESSION has atomic member values, so they can not use &(ref) constructor
// https://stackoverflow.com/questions/14182258/c11-write-move-constructor-with-atomicbool-member
// Get session of session id
SESSION* SessionManager::GetSessionPtr(INT sessionId)
{
	return m_SessionPool.at(sessionId);
}

// Post WSARecv
DWORD SessionManager::PostRecv(SESSION* pSession)
{
	if (pSession == nullptr) return WSAEINVAL;

	// TODO: consider CAS structure by checking open flag
	{
		if (pSession->IsOpened() == false) return WSAEINVAL;

		auto lpOverlapped = pSession->GetOverlapped(OP_TYPE::RECV);

		WSABUF wsabuf;
		wsabuf.buf = lpOverlapped->bufferMngr.WriteCurrent();
		wsabuf.len = lpOverlapped->bufferMngr.WritableLength();

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

		if (res == SOCKET_ERROR)
		{
			auto error = WSAGetLastError();
			if (error != WSA_IO_PENDING) return error;
		}

		m_Log->Write(LV::DEBUG, "Posted WSARecv()");
	}

	return 0;
}

// Post WSASend
DWORD SessionManager::PostSend(SESSION* pSession, size_t length)
{
	// Check condition
	if (length <= 0 || length > m_IOBufMaxSize) return WSAEMSGSIZE;
	if (pSession == nullptr) return WSAEINVAL;

	// TODO: consider CAS structure by checking open flag
	{
		if (pSession->IsOpened() == false) return WSAEINVAL;

		auto lpOverlapped = pSession->GetOverlapped(OP_TYPE::SEND);

		WSABUF wsabuf;
		wsabuf.buf = lpOverlapped->bufferMngr.ReadCurret();
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
			if (error != WSA_IO_PENDING && error != ERROR_SUCCESS) return error;
		}

		m_Log->Write(LV::DEBUG, "Posted WSARSend()");
	}

	return 0;
}
