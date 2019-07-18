#pragma once

#include <concurrent_queue.h>
#include <atomic>
#include <vector>

#include "Struct.h"
#include "IServer.h"

class SessionManager
{
public:
	SessionManager() = delete;
	SessionManager(size_t sessionNum, size_t ioMaxBufSize, IServerController* pController);

	bool retrieveId(size_t& _out_sessionId);
	void returnId(size_t sessionId);

	SESSIONDESC& GetSessionDescRef(size_t sessionId);
	LPSESSION GetSessionPtr(size_t sessionId);

public:
	static size_t	SESSION_MAX_NUMBER;

private:
	using cqueue = Concurrency::concurrent_queue<size_t>;

	size_t			m_SessionNumber;
	cqueue			m_SessionIdPool;
	std::vector<LPSESSION>	m_SessionPool;
	std::atomic_int			m_ConnectedSessionNumber;
};