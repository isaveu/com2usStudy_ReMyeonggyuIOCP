#include "Acceptor.h"
#include "AsyncIONetwork.h"
#include "AsyncIOException.h"

bool Acceptor::m_winsock_dll = false;

// Constructor of Acceptor
Acceptor::Acceptor(AsyncIONetwork* const pServer, const char* ip, const short port, Log* const pLog) 
	: m_pServer(pServer), m_ip(ip), m_port(port), m_pLogger(pLog)
{
	// Load winsock.dll
	if (m_winsock_dll == false)
	{
		WSADATA wsaData;
		WORD wVer = MAKEWORD(2, 2);
		auto ret = WSAStartup(wVer, &wsaData);
		ThrowLastErrorIf(ret != NO_ERROR, "[WSAStartup()] Error occured");

		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			WSACleanup();
			ThrowErrorIf(true, WSANOTINITIALISED, "[WSAStartup()] WSA version not matched");
		}
	}

	// Make listen socket
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ThrowLastErrorIf(m_listenSocket == INVALID_SOCKET, "[socket()] socket dosen't created");

	SOCKADDR_IN sin;
	sin.sin_port = htons(port);
	inet_pton(sin.sin_family = AF_INET, ip, &sin.sin_addr);

	// Bind listen socket to available port
	auto ret = bind(m_listenSocket, reinterpret_cast<LPSOCKADDR>(&sin), sizeof(sin));
	ThrowErrorIf(ret == SOCKET_ERROR, WSAGetLastError(), "[bind()] Fail bind");

	// Make listen wait queue
	ret = listen(m_listenSocket, 128);
	ThrowErrorIf(ret == SOCKET_ERROR, WSAGetLastError(), "[listen()] Fail listen");

	m_pLogger->Write(LV::INFO, "[%s, %d] accept started", ip, port);
}

// Accept client socket
// Acceptor thread run this function
void Acceptor::Accept()
{
	SOCKET clientSocket = INVALID_SOCKET;
	SOCKADDR_IN sockAddrIn;
	auto lpSockAddr = reinterpret_cast<LPSOCKADDR>(&sockAddrIn);
	auto len = static_cast<int>(sizeof(sockAddrIn));

	while (this->IsStart())
	{
		// TODO: change accept to AcceptEx
		// Wait until accept socket from client
		clientSocket = accept(m_listenSocket, lpSockAddr, &len);

		if (clientSocket == INVALID_SOCKET)
		{
			m_pLogger->Write(LV::ERR, "[ERROR #%u] Can not accept ", ::GetLastError());
			continue;
		}

		m_pLogger->Write(LV::INFO, "Accepted %u.%u.%u.%u, socket #%d",
			sockAddrIn.sin_addr.S_un.S_un_b.s_b1,
			sockAddrIn.sin_addr.S_un.S_un_b.s_b2,
			sockAddrIn.sin_addr.S_un.S_un_b.s_b3,
			sockAddrIn.sin_addr.S_un.S_un_b.s_b4, clientSocket);

		// If server accept socket from client,
		// register this socket to available session from server's session pool
		auto ret = m_pServer->RegisterClient(clientSocket);

		// If fail to register client socket to session, close client socket
		// This client socket can not recv, send packet anymore
		if (ret != NET_ERROR_CODE::NONE)
		{
			closesocket(clientSocket);
			m_pLogger->Write(LV::ERR, "[NET_ERROR #%d] Fail client register", ret);
		}
	}
}

// For thread function
void Acceptor::Run()
{
	Acceptor::Accept();
	m_pLogger->Write(LV::DEBUG, "Accept closed");
}

void Acceptor::Stop()
{
	m_IsStarted = false;
	closesocket(m_listenSocket);
}