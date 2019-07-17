#include "Acceptor.h"

bool Acceptor::m_ws2_32_lib = false;

Acceptor::Acceptor(AsyncIOServer* pServer, const char* ip, const u_short port) 
	: m_pServer(pServer), m_ip(ip), m_port(port), m_Log(Log::GetInstance())
{
	if (m_ws2_32_lib == false)
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
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ThrowLastErrorIf(m_listenSocket == INVALID_SOCKET, "[socket()] socket dosen't created");

	SOCKADDR_IN sin;
	sin.sin_port = htons(port);
	inet_pton(sin.sin_family = AF_INET, ip, &sin.sin_addr);

	auto res = bind(m_listenSocket, (LPSOCKADDR)& sin, sizeof(sin));
	ThrowLastErrorIf(res == SOCKET_ERROR, "[bind()] Fail bind");

	res = listen(m_listenSocket, 128);
	ThrowLastErrorIf(res == SOCKET_ERROR, "[listen()] Fail listen");
	
	m_Log->Write(utils::Format("[%s, %d] accept started\n", ip, port));
}

void Acceptor::Accept()
{
	SOCKET clientSocket = INVALID_SOCKET;
	SOCKADDR_IN addr;
	int len = sizeof(addr);

	while (this->IsStart())
	{
		clientSocket = accept(m_listenSocket, reinterpret_cast<LPSOCKADDR>(&addr), &len);

		if (clientSocket == INVALID_SOCKET)
		{
			m_Log->Write(utils::Format("[Error %u] Can not accept ", ::GetLastError()), LOG_LEVEL::ERR);
			continue;
		}

		m_Log->Write(utils::Format("Accepted %u.%u.%u.%u",
			addr.sin_addr.S_un.S_un_b.s_b1,
			addr.sin_addr.S_un.S_un_b.s_b2,
			addr.sin_addr.S_un.S_un_b.s_b3,
			addr.sin_addr.S_un.S_un_b.s_b4));

		m_pServer->registerSocket(clientSocket);
	}
}

void Acceptor::Run()
{
	Acceptor::Accept();
	m_Log->Write("Accept closed");
}