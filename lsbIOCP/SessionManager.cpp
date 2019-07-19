#include "SessionManager.h"

size_t SessionManager::SESSION_MAX_NUMBER = 3000;

// Constructor of SessionManager
// Create session object pool ( [sessionNum] sessions )
// Set unique id of each session and push id to concurrent_queue (session id queue)
SessionManager::SessionManager(size_t sessionNum, size_t ioBufMaxSize, IServerController* pController) : m_SessionNumber(sessionNum)
{
	m_ConnectedSessionNumber.store(0);
	INT sessionId = 0;
	m_SessionPool.assign(sessionNum, nullptr);
	for (auto& session : m_SessionPool)
	{
		session = new SESSION(ioBufMaxSize, pController);
		session->SetSessionId(sessionId);
		m_SessionIdPool.push(sessionId++);
	}

	Log::GetInstance()->Write(utils::Format("Create %d Sessions succesfully", sessionNum), LOG_LEVEL::DEBUG);
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
LPSESSION SessionManager::GetSessionPtr(INT sessionId)
{
	return m_SessionPool.at(sessionId);
}