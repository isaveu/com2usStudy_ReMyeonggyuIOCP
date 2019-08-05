#pragma once

#include <winsock2.h>
#include <MSWSock.h>

#include "Log.h"
#include "INetwork.h"
#include "SessionManager.h"
#include "Utils.h"
#include "Thread.h"

// For avoid cross reference of header "AsyncIONetwork.h"
// #include is declared in .cpp file
class AsyncIONetwork;

class Worker : public Thread
{
public:
	Worker() = delete;
	Worker(INetworkReceiver* const pReceiver
		, const HANDLE iocpHandle
		, SessionManager* const pSessionManager
		, AsyncIONetwork* const pServer
		, Log* const pLog);
	void Run() override;

private:
	void HandleCompletion();
	void DispatchError(DWORD error, LPOVERLAPPED lpOverlapped, int sessionId);
	void DispatchCompleteion(DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, int sessionId);

private:
	INetworkReceiver* const		m_pReceiver;
	SessionManager* const		m_pSessionManager;
	AsyncIONetwork* const	 	m_pServer;
	HANDLE		m_IOCPHandle;
	Log*		m_pLogger;
};