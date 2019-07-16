#pragma once

#include <vector>
#include <unordered_map>

#include "Utils.h"
#include "Log.h"
#include "Worker.h"
#include "Session.h"
#include "Controller.h"
#include "AsyncIOException.h"

using sessionMap = std::unordered_map<ULONG_PTR, Session*>;

class AsyncIOServer
{
public:
	AsyncIOServer() = delete;
	AsyncIOServer(IServerReceiver* pReceiver, size_t ioMaxSize, size_t threadNumber, size_t sessionNumber,  std::string name);
	~AsyncIOServer();
	void Start();
	void Stop();

private:
	IServerController* m_pController;
	IServerReceiver* m_pReceiver;

	HANDLE		m_IOCPHandle;

	size_t			m_IOMaxSize;
	static size_t	IO_MIN_SIZE;

	size_t		m_SessionNumber;
	static size_t	SESSION_MAX_NUMBER;

	size_t		m_ThreadNum;
	using workers = std::vector<std::shared_ptr<Worker>>;
	workers		m_Workers;

	std::string m_ServerName;
	Log*		m_Log;

	sessionMap	m_sessionMap;
};