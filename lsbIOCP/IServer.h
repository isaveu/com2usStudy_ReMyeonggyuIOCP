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
	// multi-thread 환경에서 호출 됨을 명심

	// 클라이언트 소켓 연결 통보
	virtual void NotifyClientConnected(SESSIONDESC& sessionDesc) const = 0;

	// 클라이언트 소켓 연결해제 통보
	virtual void NotifyClientDisconnected(short sessionId) const = 0;

	// 메시지 수신 통보
	virtual void NotifyMessage(SESSIONDESC& sessionDesc, size_t bytesNumber, char* data) const = 0;

	// 외부 서버 소켓 연결 요청 결과 통보
	virtual void NotifyServerConnectingResult(SESSIONDESC& session, size_t requestId, DWORD error) const = 0;
};