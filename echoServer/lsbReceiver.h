#pragma once

#include "../lsbIOCP/IServer.h"

class lsbReceiver : public IServerReceiver
{
	// Server can manipulate this function
	// So, when the event occurs, we can handle the logic.

	// Triggered when client socket is connected
	void NotifyClientConnected(SESSIONDESC& sessionDesc) override;

	// Triggered when socket is disconnected
	void NotifyClientDisconnected(INT sessionId) override;

	// Triggered when server get message from client
	void NotifyMessage(SESSIONDESC& sessionDesc) override;

	// Triggered when the job of connecting to other server is completed
	void NotifyServerConnectingResult(SESSIONDESC& session, INT requrestId, DWORD error) override;
};