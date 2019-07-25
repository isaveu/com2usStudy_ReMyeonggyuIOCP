#include "AsyncIOServer.h"

// Server request sending some packet data to client
DWORD AsyncIOServer::SendPacket(const INT sessionId, short length, char* data, short headerLength, char* pHeader)
{
	m_Log->Write(LV::DEBUG, "Server request: send packet");
	auto pSession = m_pSessionManager->GetSessionPtr(sessionId);
	auto overlappedEx = pSession->GetOverlapped(OP_TYPE::SEND);
	auto totalLength = length + headerLength;

	std::lock_guard<std::mutex> lock(pSession->m_SendLock);

	if (headerLength > 0)
	{
		overlappedEx->bufferMngr.Write(pHeader, 0, headerLength);
	}
	overlappedEx->bufferMngr.Write(data, 0, length);
	m_pSessionManager->PostSend(pSession, totalLength);
	return 0;
}

// Server request disconnecting current client
DWORD AsyncIOServer::DisconnectSocket(const INT sessionId)
{
	m_Log->Write(LV::DEBUG, "Server request: disconnect");
	UnlinkSocketToSession(sessionId, 0);
	return 0;
}

// Server request connecting another server
DWORD AsyncIOServer::ConnectSocket(INT requestId, const char* ip, u_short port)
{
	m_Log->Write(LV::DEBUG, "Server request: ConnectSocket");

	auto sockAddrLen = static_cast<int>(sizeof(SOCKADDR_IN));

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