#pragma once

#include <Ws2tcpip.h>

#include <vector>
#include <functional>

#include "Log.h"
#include "Utils.h"
#include "Worker.h"
#include "SessionManager.h"
#include "Acceptor.h"
#include "ErrorCode.h"
#include "AsyncIOException.h"

using workers = std::vector<std::shared_ptr<Worker>>;

class AsyncIONetwork : public INetworkController
{
public:
	AsyncIONetwork() = delete;
	AsyncIONetwork(INetworkReceiver* const pReceiver, NetworkConfig config);
	~AsyncIONetwork();
	void Start();
	void Stop();
	void Join();

	SESSION* LinkSocketToSession(const SOCKET clientSocket);
	void UnlinkSocketToSession(const int sessionId, const NET_ERROR_CODE error);
	NET_ERROR_CODE RegisterClient(const SOCKET clientSocket);

	// INetworkController
	NET_ERROR_CODE SendPacket(
		const int sessionId
		, const short length
		, char* const data
		, Message* pProto
		, const short headerLength
		, char* const header) override;
	NET_ERROR_CODE ConnectSocket(const int requestId, const char* ip, const short port) override;
	void DisconnectSocket(const int sessionId) override;

private:
	// Handler
	INetworkReceiver*	m_pReceiver;
	SessionManager*		m_pSessionManager;
	Acceptor*			m_pAcceptor;
	HANDLE				m_IOCPHandle;

	// AsyncIONetwork config
	int				m_ThreadNum;
	workers			m_Workers;
	std::string		m_ServerName;

	// Logger
	Log*			m_Log;
};