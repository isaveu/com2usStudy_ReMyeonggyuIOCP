#include "PacketProcess.h"

namespace lsbLogic
{
	void PacketProcess::Init(LogicMain* const pLogicMain
		, UserManager* const pUserMngr
		, RoomManager* const pRoomMngr
		, ServerConfig serverConfig
		, ConnectedUserManager* pConnUsrMngr
		, Log* const pLogger)
	{
		m_pLogicMain = pLogicMain;
		m_pUserMngr = pUserMngr;
		m_pRoomMngr = pRoomMngr;
		m_ServerConfig = serverConfig;
		m_pConnectedUserManager = pConnUsrMngr;
		m_Log = pLogger;

		for (int i = 0; i < static_cast<int>(PACKET_ID::MAX); i++)
		{
			PacketFuncArray[i] = nullptr;
		}

		PacketFuncArray[static_cast<int>(PACKET_ID::NTF_SYS_CONNECT_SESSION)] = &PacketProcess::NtfSysConnctSession;
		PacketFuncArray[static_cast<int>(PACKET_ID::NTF_SYS_CLOSE_SESSION)] = &PacketProcess::NtfSysCloseSession;
		PacketFuncArray[static_cast<int>(PACKET_ID::LOGIN_REQ)] = &PacketProcess::Login;
		PacketFuncArray[static_cast<int>(PACKET_ID::ROOM_ENTER_REQ)] = &PacketProcess::RoomEnter;
		PacketFuncArray[static_cast<int>(PACKET_ID::ROOM_LEAVE_REQ)] = &PacketProcess::RoomLeave;
		PacketFuncArray[static_cast<int>(PACKET_ID::ROOM_CHAT_REQ)] = &PacketProcess::RoomChat;
		PacketFuncArray[static_cast<int>(PACKET_ID::DEV_ECHO_REQ)] = &PacketProcess::DevEcho;
	}

	void PacketProcess::Process(PacketInfo packet)
	{
		auto packetId = packet.PacketId;

		if (PacketFuncArray[packetId] == nullptr)
		{
			return;
		}

		(this->*PacketFuncArray[packetId])(packet);
	}

	void PacketProcess::StateCheck()
	{
		m_pConnectedUserManager->LoginCheck();
	}

	ERROR_CODE PacketProcess::NtfSysConnctSession(PacketInfo packet)
	{
		m_pConnectedUserManager->SetConnectSession(packet.SessionId);
		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::NtfSysCloseSession(PacketInfo packet)
	{
		auto pUser = std::get<1>(m_pUserMngr->GetUser(packet.SessionId));

		if (pUser)
		{
			auto pRoom = std::get<1>(m_pRoomMngr->GetRoom(pUser->GetRoomIndex()));

			if (pRoom)
			{
				pRoom->LeaveUser(pUser->GetIndex());
				pRoom->NotifyLeaveUserInfo(pUser->GetId().c_str());
					
				m_Log->Write(LOG_LEVEL::INFO, "%s | NtfSysCloseSesson. sessionIndex(%d). Room Out", __FUNCTION__, packet.SessionId);
			}

			m_pUserMngr->RemoveUser(packet.SessionId);
		}

		m_pConnectedUserManager->SetDisConnectSession(packet.SessionId);

		m_Log->Write(LOG_LEVEL::INFO, "%s | NtfSysCloseSesson. sessionIndex(%d)", __FUNCTION__, packet.SessionId);
		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::DevEcho(PacketInfo packet)
	{
		auto packetSize = packet.PacketBodySize;
		auto reqPkt = reinterpret_cast<PacketEchoReq*>(packet.pData);

		PacketEchoRes resPkt;
		resPkt.ErrorCode = (short)ERROR_CODE::NONE;
		std::memcpy(&resPkt.Data, reqPkt->Data, packetSize);

		auto packetId = static_cast<short>(PACKET_ID::DEV_ECHO_RES);
		auto sendSize = static_cast<short>(sizeof(PacketEchoRes)) - (DEV_ECHO_DATA_MAX_SIZE - packetSize);
		auto data = reinterpret_cast<char*>(&resPkt);
		m_pLogicMain->SendMsg(packet.SessionId, packetId, static_cast<short>(sendSize), data);

		return ERROR_CODE::NONE;
	}
}