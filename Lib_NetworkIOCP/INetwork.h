#pragma once

#include <google/protobuf/message.h>

#include "Session.h"
#include "ErrorCode.h"

using namespace google::protobuf;

class INetworkController {
public:
	virtual NET_ERROR_CODE SendPacket(const int sessionId, const short length, char* const data, Message* pProto, const short headerLength, char* const header) = 0;
	virtual NET_ERROR_CODE ConnectSocket(const int requestId, const char* ip, const short port) = 0;
	virtual void DisconnectSocket(const int sessionId) = 0;
};

class INetworkReceiver {
public:
	// multi-thread ȯ�濡�� ȣ�� ���� ���

	// Ŭ���̾�Ʈ ���� ���� �뺸
	virtual void NotifyClientConnected(const int sessionId) = 0;

	// Ŭ���̾�Ʈ ���� �������� �뺸
	virtual void NotifyClientDisconnected(const int sessionId) = 0;

	// �޽��� ���� �뺸
	virtual bool NotifyMessage(const int sessionId, int nBytes, char* pData) = 0;

	// �ܺ� ���� ���� ���� ��û ��� �뺸
	virtual void NotifyServerConnectingResult(const int sessionId, int requestId, NET_ERROR_CODE error) = 0;
};