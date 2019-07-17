#include "SessionManager.h"

size_t SessionManager::SESSION_MAX_NUMBER = 3000;

// Constructor of SessionManager
// Create session object pool ( [sessionNum] sessions )
// Set unique id of each session and push id to concurrent_queue (session id queue)
SessionManager::SessionManager(size_t sessionNum, size_t ioBufMaxSize, IServerController* pController) : m_SessionNumber(sessionNum)
{
	size_t sessionId = 0;
	m_SessionPool.resize(sessionNum);
	m_SessionPool.assign(sessionNum, nullptr);
	for (auto& session : m_SessionPool)
	{
		session = std::make_shared<SESSION>(ioBufMaxSize, pController);
		session->SetSessionId(sessionId);
		m_SessionIdPool.push(sessionId++);
	}
}

bool SessionManager::retrieveId(size_t& _out_sessionId)
{
	return m_SessionIdPool.try_pop(_out_sessionId);
}

void SessionManager::returnId(size_t sessionId)
{
	m_SessionIdPool.push(sessionId);
}