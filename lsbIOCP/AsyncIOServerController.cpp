#include "AsyncIOServer.h"

// Server request sending some packet data to client
DWORD AsyncIOServer::SendPacket(SESSIONDESC& sessionDesc, size_t length, char* data)
{
	m_Log->Write("Server request: send packet", LOG_LEVEL::DEBUG);
	auto session = m_pSessionManager->GetSessionPtr(sessionDesc.id);
	PostSend(session, length, data);
	return 0;
}

// Server request disconnecting current client
DWORD AsyncIOServer::DisconnectSocket(SESSIONDESC& sessionDesc)
{
	m_Log->Write("Server request: disconnect", LOG_LEVEL::DEBUG);
	UnlinkSocketToSession(sessionDesc.id, 0);
	return 0;
}

// Server request connecting another server
DWORD AsyncIOServer::ConnectSocket(size_t requestId, const char* ip, u_short port)
{
	m_Log->Write("Server request: ConnectSocket", LOG_LEVEL::DEBUG);

	auto sockAddrLen = sizeof(SOCKADDR_IN);

	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sockAddrLen);

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = port;
	inet_pton(sockAddr.sin_family = AF_INET, ip, &sockAddr.sin_addr);

	// Make socket
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) return GetLastError();

	SOCKADDR_IN localAddr;
	ZeroMemory(&localAddr, sockAddrLen);

	localAddr.sin_family = AF_INET;

	// bind socket
	auto resBind = bind(clientSocket, reinterpret_cast<LPSOCKADDR>(&localAddr), sockAddrLen);
	if (resBind == SOCKET_ERROR) return WSAGetLastError();

	// Load ConnectEx function by calling WSAIoctl
	LPFN_CONNECTEX ConnectEx = 0;
	GUID guid = WSAID_CONNECTEX;
	DWORD ctls = 0;
	auto resWSAIoctl = WSAIoctl(
		clientSocket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guid,
		sizeof(GUID),
		&ConnectEx,
		sizeof(LPFN_CONNECTEX),
		&ctls,
		NULL,
		NULL);
	if (resWSAIoctl != NULL) return WSAGetLastError();

	// pSession->EnterIO();

	// Reserve available session with socket
	auto pSession = LinkSocketToSession(clientSocket);
	if (pSession == nullptr) return WSAENOBUFS;

	// Socket is not connected other host yet
	// So, don't open the session util setsockopt() is called in GQCS job

	auto lpOverlapped = pSession->GetOverlapped(OP_TYPE::CONN);
	lpOverlapped->requesterId = requestId;

	// 6387: overlapped is already allocated with constructor
	// Post connecting job to IOCP
#pragma warning(disable:6387)
	auto resConnEx = ConnectEx(
		clientSocket,
		reinterpret_cast<LPSOCKADDR>(&sockAddr),
		sockAddrLen,
		NULL, NULL, NULL,
		&lpOverlapped->overlapped);
#pragma warning(default:6387)

	if (resConnEx == FALSE)
	{
		auto error = WSAGetLastError();
		if (error != ERROR_IO_PENDING)
		{
			UnlinkSocketToSession(pSession->GetSessionId(), error);
			return error;
		}
	}

	return 0;
}