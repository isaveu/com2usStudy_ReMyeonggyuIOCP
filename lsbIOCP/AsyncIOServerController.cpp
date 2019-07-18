#include "AsyncIOServer.h"

DWORD AsyncIOServer::SendPacket(SESSIONDESC& sessionDesc, size_t length, char* data)
{
	auto session = m_pSessionManager->GetSessionPtr(sessionDesc.id);
	PostSend(session, length, data);
	return 0;
}

DWORD AsyncIOServer::DisconnectSocket(SESSIONDESC& sessionDesc)
{
	ReleaseSession(sessionDesc.id, 0);
	return 0;
}

DWORD AsyncIOServer::ConnectSocket(size_t requestId, std::string ip, u_short port)
{
	return 0;
}