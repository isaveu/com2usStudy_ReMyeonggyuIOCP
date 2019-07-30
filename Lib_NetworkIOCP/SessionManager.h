#pragma once

#include <concurrent_queue.h>
#include <atomic>
#include <vector>

#include "Log.h"
#include "Session.h"
#include "INetwork.h"
#include "AsyncIOException.h"

class SessionManager
{
public:
	SessionManager() = delete;
	SessionManager(const int sessionNum, const PacketBufferConfig pktBufferConfig, Log* const pLog);
	~SessionManager();

	bool retrieveId(int& _out_sessionId);
	void returnId(int sessionId);

	SESSION* GetSessionPtr(int sessionId);

	NET_ERROR_CODE PostRecv(SESSION* session);
	NET_ERROR_CODE PostSend(SESSION* session, int length);

private:
	using cqueue = Concurrency::concurrent_queue<int>;
	int				m_IOBufMaxSize;
	int				m_SessionNumber;
	cqueue			m_SessionIdPool;
	std::vector<SESSION*>	m_SessionPool;
	std::atomic_int			m_ConnectedSessionNumber;

	Log* m_pLogger;
};