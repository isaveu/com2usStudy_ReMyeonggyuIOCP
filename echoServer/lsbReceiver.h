#pragma once

#include "../lsbIOCP/IServer.h"

class lsbReceiver : public IServerReceiver
{
	// 클라이언트 소켓 연결 통보
	void NotifyClientConnected() const override;

	// 클라이언트 소켓 연결해제 통보
	void NotifyClientDisconnected() const override;

	// 메시지 수신 통보
	void NotifyMessage() const override;

	// 외부 서버 소켓 연결 요청 결과 통보
	void NotifyServerConnectingResult() const override;

	// 외부 서버 소켓 연결해제 통보
	void NotifyServerDisconnect() const override;
};