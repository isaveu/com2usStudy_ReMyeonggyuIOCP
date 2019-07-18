#pragma once

#include <vector>
#include <functional>

#include "Log.h"
#include "Utils.h"
#include "Worker.h"
#include "SessionManager.h"
#include "AsyncIOException.h"

using workers = std::vector<std::shared_ptr<Worker>>;

class AsyncIOServer : public IServerController
{
public:
	AsyncIOServer() = delete;
	AsyncIOServer(IServerReceiver* pReceiver, DWORD ioMaxSize, DWORD threadNumber, DWORD sessionNumber,  std::string name);
	~AsyncIOServer();
	void Start();
	void Stop();
	void Join();

	DWORD RegisterSession(SOCKET clientSocket);
	DWORD ReleaseSession(size_t sessionId, DWORD error);
	DWORD PostRecv(SESSION* session);
	DWORD PostSend(SESSION* session, size_t length, char* data);

	// IServerController
	DWORD SendPacket(SESSIONDESC& sessionDesc, size_t length, char* data) override;
	DWORD ConnectSocket(size_t requestId, std::string ip, u_short port) override;
	DWORD DisconnectSocket(SESSIONDESC& sessionDesc) override;

private:
	IServerReceiver*	m_pReceiver;

	HANDLE			m_IOCPHandle;

	static size_t	IO_MIN_SIZE;
	size_t			m_IOMaxSize;

	size_t			m_ThreadNum;
	workers			m_Workers;

	std::string		m_ServerName;
	Log*			m_Log;

	SessionManager*		m_pSessionManager;
};