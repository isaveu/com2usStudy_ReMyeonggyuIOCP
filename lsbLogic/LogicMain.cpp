#include "..//lsbIOCP/AsyncIONetwork.h"

#include "ConnectedUserManager.h"
#include "UserManager.h"
#include "RoomManager.h"
#include "PacketProcess.h"
#include "LogicMain.h"

namespace lsbLogic
{
	void LogicMain::Start()
	{
		m_pNetwork->Start();
		m_IsRun = true;
	}

	void LogicMain::Stop()
	{
		m_pNetwork->Stop();
		m_IsRun = false;
	}

	void LogicMain::Run()
	{
		m_pLogger->Write(LV::TRACE, "%s | Run packet proc", __FUNCTION__);
		while (m_IsRun)
		{
			PacketInfo packetInfo;
			if (m_PacketQueue.try_pop(packetInfo) == false)
			{
				m_pConnUserMngr->LoginCheck();
				continue;
			}

			m_pPktProc->Process(packetInfo);
		}
		m_pLogger->Write(LV::TRACE, "%s | Exit packet proc", __FUNCTION__);
	}

	void LogicMain::Init(LogicConfig config)
	{
		m_Config = config;

		NetworkConfig networkConfig =
		{
			config.threadNumber,
			config.sessionNumber,
			config.bufferSize,
			PACKET_HEADER_SIZE,
			PACKET_MAX_SIZE,
			config.ip,
			config.port,
			config.name
		};

		m_pNetwork = new AsyncIONetwork(this, networkConfig);

		m_pLogger = new Log();
		auto file = std::string("tempLog");
		m_pLogger->Init(LV::DEBUG, file);

		m_pUserMngr = new UserManager();
		m_pUserMngr->Init(m_Config.maxUserNum);

		m_pRoomMngr = new RoomManager();
		m_pRoomMngr->Init(this, m_Config.maxRoomNum, m_pLogger);

		m_pConnUserMngr = new ConnectedUserManager();
		m_pConnUserMngr->Init(m_pNetwork, m_Config.sessionNumber, true, m_pLogger);

		m_pPktProc = new PacketProcess();
		m_pPktProc->Init(this, m_pUserMngr, m_pRoomMngr, m_pConnUserMngr, m_pLogger);

		m_pLogger->Write(LOG_LEVEL::INFO, "%s | Init Success", __FUNCTION__);
	}

	void LogicMain::SendMsg(const int sessionId, const short packetId, const short length, char* pData, Message* pMsg)
	{
		// TODO: packet header를 각 res packet에 상속시켜 사용 -> Write 두번 할 필요가 없어짐
		auto totalSize = static_cast<short>(PACKET_HEADER_SIZE + length);
		PacketHeader header{ totalSize, packetId, static_cast<unsigned char>(0) };
		m_pLogger->Write(LV::DEBUG, "%s | packet size : %u, packet id : %u, body length %u", __FUNCTION__, totalSize, packetId, length);
		m_pNetwork->SendPacket(sessionId, length, pData, pMsg, PACKET_HEADER_SIZE, reinterpret_cast<char*>(&header));
		m_pLogger->Write(LV::TRACE, "%s | Send Packet", __FUNCTION__);
	}

	void LogicMain::SendProto(const int sessionId, const short packetId, Message* pMsg)
	{
		auto bodyLength = static_cast<short>(pMsg->ByteSize());
		auto totalSize = static_cast<short>(PACKET_HEADER_SIZE + bodyLength);
		PacketHeader header{ totalSize, packetId, static_cast<unsigned char>(0) };
		m_pLogger->Write(LV::DEBUG, "%s | packet size : %u, packet id : %u, body length %u", __FUNCTION__, totalSize, packetId, bodyLength);
		m_pNetwork->SendPacket(sessionId, bodyLength, nullptr, pMsg, PACKET_HEADER_SIZE, reinterpret_cast<char*>(&header));
		m_pLogger->Write(LV::TRACE, "%s | Send Proto", __FUNCTION__);
	}

	void LogicMain::ForceClose(const int sessionId)
	{
		m_pNetwork->DisconnectSocket(sessionId);
		m_pLogger->Write(LV::TRACE, "%s | Close connection", __FUNCTION__);
	}

	void LogicMain::ConnectServer(const int reqId, const char* ip, unsigned short port)
	{
		m_pNetwork->ConnectSocket(reqId, ip, port);
		m_pLogger->Write(LV::TRACE, "%s | Request connection to server %s - %u", __FUNCTION__, ip, port);
	}
}