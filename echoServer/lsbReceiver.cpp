#include "lsbReceiver.h"

void lsbReceiver::NotifyClientConnected(SESSIONDESC& sessionDesc) const
{
	printf("Connected %d\n", sessionDesc.id);
}

void lsbReceiver::NotifyClientDisconnected(short sessionId) const
{
	printf("Disconnected %d\n", sessionId);
}

void lsbReceiver::NotifyMessage(SESSIONDESC& sessionDesc, size_t bytesNumber, char* data) const
{
	char* pbuffer = new char[bytesNumber];
	memcpy(pbuffer, data, bytesNumber);

	if ('q' == pbuffer[0])
		sessionDesc.pController->DisconnectSocket(sessionDesc);
	else
		sessionDesc.pController->SendPacket(sessionDesc, bytesNumber, pbuffer);

	delete[] pbuffer;
}

void lsbReceiver::NotifyServerConnectingResult(SESSIONDESC& session, size_t requestId, DWORD error) const
{
	
}