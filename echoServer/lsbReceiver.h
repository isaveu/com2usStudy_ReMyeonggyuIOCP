#pragma once

#include "../lsbIOCP/IServer.h"

class lsbReceiver : public IServerReceiver
{
	// 클라이언트 소켓 연결 통보
	void NotifyClientConnected(SESSIONDESC& sessionDesc) const override;

	// 클라이언트 소켓 연결해제 통보
	void NotifyClientDisconnected(short sessionId) const override;

	// 메시지 수신 통보
	void NotifyMessage(SESSIONDESC& sessionDesc, size_t bytesNumber, char* data) const override;

	// 외부 서버 소켓 연결 요청 결과 통보
	void NotifyServerConnectingResult(SESSIONDESC& session, size_t requrestId, DWORD error) const override;
};