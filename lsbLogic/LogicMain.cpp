#include "..//lsbIOCP/AsyncIOServer.h"

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
	}

	void LogicMain::Init(ServerConfig serverConfig, LogicConfig logicConfig)
	{
		m_SConfig = serverConfig;
		m_LConfig = logicConfig;

		m_pNetwork = new AsyncIOServer(this, serverConfig);
		
		m_Log = new Log();
		auto file = std::string("tempLog");
		m_Log->Init(LV::DEBUG, file);

		m_pUserMngr = new UserManager();
		m_pUserMngr->Init(logicConfig.maxUserNum);

		m_pRoomMngr = new RoomManager();
		m_pRoomMngr->Init(logicConfig.maxRoomNum, logicConfig.maxUserNumInRoom);

		m_pConnUserMngr = new ConnectedUserManager();
		m_pConnUserMngr->Init(m_pNetwork, serverConfig.sessionNumber, true, m_Log);

		m_pPktProc = new PacketProcess();
		m_pPktProc->Init(this, m_pUserMngr, m_pRoomMngr, serverConfig, m_pConnUserMngr, m_Log);

		m_Log->Write(LOG_LEVEL::INFO, "%s | Init Success", __FUNCTION__);
	}

	void LogicMain::SendMsg(const int sessionId, const short packetId, const short length, char* data)
	{
		auto totalSize = static_cast<short>(PACKET_HEADER_SIZE + length);
		PacketHeader header{ totalSize, packetId, static_cast<unsigned char>(0) };
		m_pNetwork->SendPacket(sessionId, length, data, PACKET_HEADER_SIZE, reinterpret_cast<char*>(&header));
	}

	void LogicMain::ForceClose(const int sessionId)
	{
		m_pNetwork->DisconnectSocket(sessionId);
	}

	void LogicMain::ConnectServer(const int reqId, const char* ip, unsigned short port)
	{
		m_pNetwork->ConnectSocket(reqId, ip, port);
	}
}