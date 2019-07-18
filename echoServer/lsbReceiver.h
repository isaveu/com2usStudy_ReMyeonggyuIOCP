#pragma once

#include "../lsbIOCP/IServer.h"

class lsbReceiver : public IServerReceiver
{
	// Ŭ���̾�Ʈ ���� ���� �뺸
	void NotifyClientConnected(SESSIONDESC& sessionDesc) const override;

	// Ŭ���̾�Ʈ ���� �������� �뺸
	void NotifyClientDisconnected(short sessionId) const override;

	// �޽��� ���� �뺸
	void NotifyMessage(SESSIONDESC& sessionDesc, size_t bytesNumber, char* data) const override;

	// �ܺ� ���� ���� ���� ��û ��� �뺸
	void NotifyServerConnectingResult(SESSIONDESC& session, size_t requrestId, DWORD error) const override;
};