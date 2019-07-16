#pragma once

#include "../lsbIOCP/IServer.h"

class lsbReceiver : public IServerReceiver
{
	// Ŭ���̾�Ʈ ���� ���� �뺸
	void notifyClientConnected() const;

	// Ŭ���̾�Ʈ ���� �������� �뺸
	void notifyClientDisconnected() const;

	// �޽��� ���� �뺸
	void notifyMessage() const = 0;

	// �ܺ� ���� ���� ���� ��û ��� �뺸
	void notifyServerConnectingResult() const;

	// �ܺ� ���� ���� �������� �뺸
	void notifyServerDisconnect() const;
};