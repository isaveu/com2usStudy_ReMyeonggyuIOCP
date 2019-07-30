#include "AsyncIONetwork.h"

// Server request sending some packet data to client
NET_ERROR_CODE AsyncIONetwork::SendPacket(
	const int sessionId
	, const short length
	, char* const data
	, Message* pProto
	, const short headerLength
	, char* const pHeader)
{
	m_Log->Write(LV::DEBUG, "Server request: send packet");
	auto pSession = m_pSessionManager->GetSessionPtr(sessionId);
	auto overlappedEx = pSession->GetOverlapped(OP_TYPE::SEND);
	auto totalLength = length + headerLength;

	std::lock_guard<std::mutex> lock(pSession->m_SendLock);

	// ������� ���� write ���� üũ ���ص� ������ ������ ��¥�� body���� �߻���
	if (headerLength > 0)
	{
		overlappedEx->bufferMngr.Write(pHeader, nullptr, 0, headerLength, false);
	}

	auto isSuccess = overlappedEx->bufferMngr.Write(data, pProto, 0, length);
	if (isSuccess == false)
	{
		// ���۸� �� ä�︸ŭ ����� ����� �̷����� �ʴ� ��Ȳ�̹Ƿ� ������ �����Ѵ�.
		auto error = NET_ERROR_CODE::PACKET_BUFFER_FULL;
		UnlinkSocketToSession(sessionId, error);
		return error;
	}

	return m_pSessionManager->PostSend(pSession, totalLength);
}

// Server request disconnecting current client
void AsyncIONetwork::DisconnectSocket(const int sessionId)
{
	m_Log->Write(LV::DEBUG, "Server request: disconnect");
	UnlinkSocketToSession(sessionId, NET_ERROR_CODE::NONE);
}

// Server request connecting another server
NET_ERROR_CODE AsyncIONetwork::ConnectSocket(const int requestId, const char* ip, const short port)
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
	if (clientSocket == INVALID_SOCKET)
	{
		return NET_ERROR_CODE::FAIL_SOCKET_CREATE;
	}

	SOCKADDR_IN localAddr;
	ZeroMemory(&localAddr, sockAddrLen);

	localAddr.sin_family = AF_INET;

	// bind socket
	auto resBind = bind(clientSocket, reinterpret_cast<LPSOCKADDR>(&localAddr), sockAddrLen);
	if (resBind == SOCKET_ERROR)
	{
		return NET_ERROR_CODE::FAIL_SOCKET_BIND;
	}

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
	if (resWSAIoctl != NULL)
	{
		return NET_ERROR_CODE::FAIL_CONNECTEX_LOAD;
	}

	// pSession->EnterIO();

	// Reserve available session with socket
	auto pSession = LinkSocketToSession(clientSocket);
	if (pSession == nullptr)
	{
		return NET_ERROR_CODE::FAIL_LINK_SOCKET_TO_SESSION;
	}

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

	if (resConnEx == FALSE && WSAGetLastError() != ERROR_IO_PENDING)
	{
		auto error = NET_ERROR_CODE::FAIL_CONNECTEX_CALL;
		UnlinkSocketToSession(pSession->GetSessionId(), error);
		return error;
	}

	return NET_ERROR_CODE::NONE;
}