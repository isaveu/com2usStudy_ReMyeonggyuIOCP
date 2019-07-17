#pragma once

#include <vector>
#include <unordered_map>

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

	void registerSocket(SOCKET clientSocket);
	void postRecv(SESSION* session);

	// IServerController
	DWORD postSend() override;
	DWORD disconnectSocket() override;
	DWORD connectSocket() override;

private:
	IServerReceiver*	m_pReceiver;

	HANDLE			m_IOCPHandle;

	static size_t	IO_MIN_SIZE;
	size_t			m_IOMaxSize;

	size_t			m_ThreadNum;
	workers			m_Workers;

	std::string		m_ServerName;
	Log*			m_Log;

	SessionManager*		m_pSessinManager;
};