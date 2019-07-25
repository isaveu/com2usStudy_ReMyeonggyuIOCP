#include "lsbReceiver.h"

void lsbReceiver::NotifyClientConnected(SESSIONDESC& sessionDesc)
{
	printf("Connected %d\n", sessionDesc.id);
}

void lsbReceiver::NotifyClientDisconnected(INT sessionId)
{
	printf("Disconnected %d\n", sessionId);
}

void lsbReceiver::NotifyMessage(SESSIONDESC& sessionDesc, size_t bytesNumber, char* data)
{
	char* pbuffer = new char[bytesNumber];
	memcpy(pbuffer, data, bytesNumber);

	if ('q' == pbuffer[0])
		sessionDesc.pController->DisconnectSocket(sessionDesc.id);
	else
		sessionDesc.pController->SendPacket(sessionDesc.id, (int)bytesNumber, pbuffer, 0, 0);

	delete[] pbuffer;
}

void lsbReceiver::NotifyServerConnectingResult(SESSIONDESC& sessionDesc, INT requestId, DWORD error)
{
	if (error != FALSE)
		printf("connecting fail, error %d\n", error);
	else
		printf("connecting successfully session id : %d, req Id : %d\n", sessionDesc.id, requestId);
}