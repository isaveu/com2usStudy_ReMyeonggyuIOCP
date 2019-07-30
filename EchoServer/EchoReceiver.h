#pragma once

#include "../Lib_NetworkIOCP/INetwork.h"

class EchoReceiver : public INetworkReceiver
{
public:
	void Init(INetworkController* pController) { m_pNetwork = pController; }

private:
	void NotifyClientConnected(const int sessionId) override
	{
		printf("Connected session Id %d\n", sessionId);
	}

	// 클라이언트 소켓 연결해제 통보
	void NotifyClientDisconnected(const int sessionId) override
	{
		printf("Disconnected session Id %d\n", sessionId);
	}

	// 메시지 수신 통보
	bool NotifyMessage(const int sessionId, int nBytes, char* pData) override
	{
		printf("Got message size %d bytes\n", nBytes);
		m_pNetwork->SendPacket(sessionId, (short)nBytes, pData, nullptr, 0, nullptr);
		return true;
	}

	// 외부 서버 소켓 연결 요청 결과 통보
	void NotifyServerConnectingResult(const int sessionId, int requestId, NET_ERROR_CODE error) override {}

private:
	INetworkController* m_pNetwork;
};