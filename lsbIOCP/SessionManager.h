#pragma once

#include <concurrent_queue.h>
#include <mutex>
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

	// TODO: session info getter

public:
	static size_t	SESSION_MAX_NUMBER;

private:
	using cqueue = Concurrency::concurrent_queue<size_t>;
	using sessionPool = std::vector<std::shared_ptr<SESSION>>;
	cqueue			m_SessionIdPool;
	sessionPool		m_SessionPool;
	size_t			m_SessionNumber;
};