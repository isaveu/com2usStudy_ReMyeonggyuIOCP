#pragma once

#include "Struct.h"

class IServerController {
public:
	virtual DWORD SendPacket(SESSIONDESC& sessionDesc, size_t length, char* data) = 0;
	virtual DWORD ConnectSocket(size_t requestId, std::string ip, u_short port) = 0;
	virtual DWORD DisconnectSocket(SESSIONDESC& sessionDesc) = 0;
};

class IServerReceiver {
public:
	// multi-thread ȯ�濡�� ȣ�� ���� ���

	// Ŭ���̾�Ʈ ���� ���� �뺸
	virtual void NotifyClientConnected(SESSIONDESC& sessionDesc) const = 0;

	// Ŭ���̾�Ʈ ���� �������� �뺸
	virtual void NotifyClientDisconnected(short sessionId) const = 0;

	// �޽��� ���� �뺸
	virtual void NotifyMessage(SESSIONDESC& sessionDesc, size_t bytesNumber, char* data) const = 0;

	// �ܺ� ���� ���� ���� ��û ��� �뺸
	virtual void NotifyServerConnectingResult(SESSIONDESC& session, size_t requestId, DWORD error) const = 0;
};