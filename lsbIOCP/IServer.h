#pragma once

#include "Session.h"

class IServerController {
public:
	virtual DWORD SendPacket(const INT sessionId, short length, char* data, short headerLength, char* header) = 0;
	virtual DWORD ConnectSocket(INT requestId, const char* ip, u_short port) = 0;
	virtual DWORD DisconnectSocket(const INT sessionId) = 0;
};

class IServerReceiver {
public:
	// multi-thread ȯ�濡�� ȣ�� ���� ���

	// Ŭ���̾�Ʈ ���� ���� �뺸
	virtual void NotifyClientConnected(SESSIONDESC& sessionDesc) = 0;

	// Ŭ���̾�Ʈ ���� �������� �뺸
	virtual void NotifyClientDisconnected(INT sessionId) = 0;

	// �޽��� ���� �뺸
	virtual bool NotifyMessage(SESSIONDESC& sessionDesc, size_t nBytes, char* pData) = 0;

	// �ܺ� ���� ���� ���� ��û ��� �뺸
	virtual void NotifyServerConnectingResult(SESSIONDESC& session, INT requestId, DWORD error) = 0;
};