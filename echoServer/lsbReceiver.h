#pragma once

#include "../lsbIOCP/IServer.h"

class lsbReceiver : public IServerReceiver
{
	// Server can manipulate this function
	// So, when the event occurs, we can handle the logic.

	// Triggered when client socket is connected
	void NotifyClientConnected(SESSIONDESC& sessionDesc) const override;

	// Triggered when socket is disconnected
	void NotifyClientDisconnected(short sessionId) const override;

	// Triggered when server get message from client
	void NotifyMessage(SESSIONDESC& sessionDesc, size_t bytesNumber, char* data) const override;

	// Triggered when the job of connecting to other server is completed
	void NotifyServerConnectingResult(SESSIONDESC& session, INT requrestId, DWORD error) const override;
};