#include "lsbLogicMain.h"

namespace lsbLogic
{
	void lsbLogicMain::NotifyClientConnected(SESSIONDESC& sessionDesc) const
	{
		printf("Connected %d\n", sessionDesc.id);
	}

	void lsbLogicMain::NotifyClientDisconnected(INT sessionId) const
	{
		printf("Disconnected %d\n", sessionId);
	}

	void lsbLogicMain::NotifyMessage(SESSIONDESC& sessionDesc, size_t bytesNumber, char* data) const
	{
		int sessionId = sessionDesc.id;
		// 유저의 recv 패킷 버퍼에 write (packetBufferManager 이용)
		// 유저의 recv 패킷 버퍼를 읽어서 하나의 패킷 단위 이상 들어왔을 경우 process
		// 아닐 시 return
	}

	void lsbLogicMain::NotifyServerConnectingResult(SESSIONDESC& sessionDesc, INT requestId, DWORD error) const
	{
		if (error != FALSE)
			printf("connecting fail, error %d\n", error);
		else
			printf("connecting successfully session id : %d, req Id : %d\n", sessionDesc.id, requestId);
	}
}