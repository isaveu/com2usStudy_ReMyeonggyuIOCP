#pragma once

#include <concurrent_queue.h>
#include <atomic>
#include <vector>

#include "Log.h"
#include "Session.h"
#include "IServer.h"
#include "AsyncIOException.h"

class SessionManager
{
public:
	SessionManager() = delete;
	SessionManager(
		const INT sessionNum
		, SessionConfig sessionConfig
		, PacketBufferConfig pktBufferConfig
		, Log* const pLog);

	bool retrieveId(INT& _out_sessionId);
	void returnId(INT sessionId);

	SESSIONDESC& GetSessionDescRef(INT sessionId);
	SESSION* GetSessionPtr(INT sessionId);

	DWORD PostRecv(SESSION* session);
	DWORD PostSend(SESSION* session, size_t length);

private:
	using cqueue = Concurrency::concurrent_queue<INT>;
	INT				m_IOBufMaxSize;
	INT				m_SessionNumber;
	cqueue			m_SessionIdPool;
	std::vector<SESSION*>	m_SessionPool;
	std::atomic_int			m_ConnectedSessionNumber;

	Log* m_Log;
};