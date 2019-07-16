#pragma once

#include "../lsbIOCP/IServer.h"

class lsbReceiver : public IServerReceiver
{
	// 클라이언트 소켓 연결 통보
	void notifyClientConnected() const;

	// 클라이언트 소켓 연결해제 통보
	void notifyClientDisconnected() const;

	// 메시지 수신 통보
	void notifyMessage() const = 0;

	// 외부 서버 소켓 연결 요청 결과 통보
	void notifyServerConnectingResult() const;

	// 외부 서버 소켓 연결해제 통보
	void notifyServerDisconnect() const;
};