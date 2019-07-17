#pragma once

#include "Struct.h"
#include "IServer.h"
#include "SessionManager.h"
#include "Log.h"
#include "Utils.h"
#include "Thread.h"

class Worker : public Thread
{
public:
	Worker() = delete;
	Worker(IServerReceiver* pReceiver, HANDLE iocpHandle, SessionManager* sessionManager);
	void Run() override;

private:
	void HandleCompletion();
	void dispatchError(DWORD error, DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, ULONG_PTR id);
	void dispatchCompleteion(DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, ULONG_PTR id);

private:
	IServerReceiver*	m_pReceiver;
	SessionManager*		m_pSessionManager;
	HANDLE				m_IOCPHandle;
	Log*				m_Log;
};