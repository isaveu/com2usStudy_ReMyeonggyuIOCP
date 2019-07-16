#pragma once

#include <windows.h>

#include "AsyncIOServer.h"

class IServerController {
public:
	virtual DWORD postRecv() = 0;
	virtual DWORD postSend() = 0;
	virtual DWORD disconnectSocket() = 0;
	virtual DWORD connectSocket() = 0;
	virtual DWORD registerSokcet() = 0;
};

class IServerReceiver {
public:
	// multi-thread ȯ�濡�� ȣ�� ���� ���

	// Ŭ���̾�Ʈ ���� ���� �뺸
	virtual void NotifyClientConnected() const = 0;

	// Ŭ���̾�Ʈ ���� �������� �뺸
	virtual void NotifyClientDisconnected() const = 0;

	// �޽��� ���� �뺸
	virtual void NotifyMessage() const = 0;

	// �ܺ� ���� ���� ���� ��û ��� �뺸
	virtual void NotifyServerConnectingResult() const = 0;

	// �ܺ� ���� ���� �������� �뺸
	virtual void NotifyServerDisconnect() const = 0;

	// Get session map information
	virtual sessionMap GetSessionMap() const = 0;
};