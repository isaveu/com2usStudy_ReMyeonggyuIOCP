#include "LogicMain.h"

namespace lsbLogic
{
	void LogicMain::NotifyClientConnected(SESSIONDESC& sessionDesc)
	{
		PacketInfo packet;
		packet.SessionId = sessionDesc.id;
		packet.PacketId = static_cast<short>(PACKET_ID::NTF_SYS_CONNECT_SESSION);
		m_PacketQueue.push(packet);
	}

	void LogicMain::NotifyClientDisconnected(INT sessionId) 
	{
		PacketInfo packet;
		packet.SessionId = sessionId;
		packet.PacketId = static_cast<short>(PACKET_ID::NTF_SYS_CLOSE_SESSION);
		m_PacketQueue.push(packet);
	}

	bool LogicMain::NotifyMessage(SESSIONDESC& sessionDesc, size_t size, char* data)
	{
		// 유저의 recv 패킷 버퍼에 write (packetBufferManager 이용)
		// 유저의 recv 패킷 버퍼를 읽어서 하나의 패킷 단위 이상 들어왔을 경우 process
		// 아닐 시 return

		if (size < PACKET_HEADER_SIZE)
		{
			return false;
		}

		PacketHeader* pPktHeader = reinterpret_cast<PacketHeader*>(data);
		auto totalSize = pPktHeader->PacketSize;

		if (size < totalSize)
		{
			return false;
		}

		PacketInfo packet;
		packet.SessionId = sessionDesc.id;
		packet.PacketId = pPktHeader->Id;
		packet.PacketBodySize = pPktHeader->PacketSize - PACKET_HEADER_SIZE;
		packet.pData = data + PACKET_HEADER_SIZE;

		m_PacketQueue.push(packet);

		return true;
	}

	void LogicMain::NotifyServerConnectingResult(SESSIONDESC& sessionDesc, INT requestId, DWORD error)
	{
		if (error != FALSE)
			printf("connecting fail, error %d\n", error);
		else
			printf("connecting successfully session id : %d, req Id : %d\n", sessionDesc.id, requestId);
	}
}