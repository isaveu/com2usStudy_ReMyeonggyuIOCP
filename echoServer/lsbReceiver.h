#pragma once

#include "../lsbIOCP/IServer.h"

class lsbReceiver : public IServerReceiver
{
	// Ŭ���̾�Ʈ ���� ���� �뺸
	void NotifyClientConnected() const override;

	// Ŭ���̾�Ʈ ���� �������� �뺸
	void NotifyClientDisconnected() const override;

	// �޽��� ���� �뺸
	void NotifyMessage() const override;

	// �ܺ� ���� ���� ���� ��û ��� �뺸
	void NotifyServerConnectingResult() const override;

	// �ܺ� ���� ���� �������� �뺸
	void NotifyServerDisconnect() const override;
};