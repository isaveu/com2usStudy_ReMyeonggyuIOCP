#include "Acceptor.h"
#include "AsyncIOServer.h"

bool Acceptor::m_winsock_dll = false;

// Constructor of Acceptor
Acceptor::Acceptor(AsyncIOServer* pServer, const char* ip, const u_short port, Log* pLog) 
	: m_pServer(pServer), m_ip(ip), m_port(port), m_Log(pLog)
{
	// Load winsock.dll
	if (m_winsock_dll == false)
	{
		WSADATA wsaData;
		WORD wVer = MAKEWORD(2, 2);
		ThrowLastErrorIf(WSAStartup(wVer, &wsaData) != NO_ERROR, "[WSAStartup()] Error occured");
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
	auto res = bind(m_listenSocket, reinterpret_cast<LPSOCKADDR>(&sin), sizeof(sin));
	ThrowErrorIf(res == SOCKET_ERROR, WSAGetLastError(), "[bind()] Fail bind");

	// Make listen wait queue
	res = listen(m_listenSocket, 128);
	ThrowErrorIf(res == SOCKET_ERROR, WSAGetLastError(), "[listen()] Fail listen");
	
	m_Log->Write(LV::INFO, "[%s, %d] accept started", ip, port);
}

// Accept client socket
// Acceptor thread run this function
void Acceptor::Accept()
{
	SOCKET clientSocket = INVALID_SOCKET;
	SOCKADDR_IN addr;
	int len = sizeof(addr);

	while (this->IsStart())
	{
		// TODO: change accept to AcceptEx
		// Wait until accept socket from client
		clientSocket = accept(m_listenSocket, reinterpret_cast<LPSOCKADDR>(&addr), &len);

		if (clientSocket == INVALID_SOCKET)
		{
			m_Log->Write(LV::ERR, "#%u Can not accept ", ::GetLastError());
			continue;
		}

		m_Log->Write(LV::INFO, "Accepted %u.%u.%u.%u, socket %d",
			addr.sin_addr.S_un.S_un_b.s_b1,
			addr.sin_addr.S_un.S_un_b.s_b2,
			addr.sin_addr.S_un.S_un_b.s_b3,
			addr.sin_addr.S_un.S_un_b.s_b4, clientSocket);

		// If server accept socket from client,
		// register this socket to available session from server's session pool
		auto error = m_pServer->RegisterClient(clientSocket);

		// If fail to register client socket to session, close client socket
		// This client socket can not recv, send packet anymore
		if (error != FALSE)
		{
			closesocket(clientSocket);
			m_Log->Write(LV::ERR, "#%u Fail client register", error);
		}
	}
}

// For thread function
void Acceptor::Run()
{
	Acceptor::Accept();
	m_Log->Write(LV::DEBUG, "Accept closed");
}