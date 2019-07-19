#pragma once

#include <concurrent_queue.h>
#include <atomic>
#include <vector>

#include "Log.h"
#include "Struct.h"
#include "IServer.h"

class SessionManager
{
public:
	SessionManager() = delete;
	SessionManager(size_t sessionNum, size_t ioMaxBufSize, IServerController* pController);

	bool retrieveId(INT& _out_sessionId);
	void returnId(INT sessionId);

	SESSIONDESC& GetSessionDescRef(INT sessionId);
	LPSESSION GetSessionPtr(INT sessionId);

public:
	static size_t	SESSION_MAX_NUMBER;

private:
	using cqueue = Concurrency::concurrent_queue<INT>;

	size_t			m_SessionNumber;
	cqueue			m_SessionIdPool;
	std::vector<LPSESSION>	m_SessionPool;
	std::atomic_int			m_ConnectedSessionNumber;
};