#include "PacketProcess.h"

namespace lsbLogic
{
	void PacketProcess::Init(LogicMain* const pLogicMain
		, UserManager* const pUserMngr
		, RoomManager* const pRoomMngr
		, ConnectedUserManager* pConnUsrMngr
		, Log* const pLogger)
	{
		m_pLogicMain = pLogicMain;
		m_pUserMngr = pUserMngr;
		m_pRoomMngr = pRoomMngr;
		m_pConnectedUserManager = pConnUsrMngr;
		m_Log = pLogger;

		for (int i = 0; i < static_cast<int>(PACKET_ID::MAX); i++)
		{
			PacketFuncArray[i] = nullptr;
		}

		PacketFuncArray[static_cast<int>(PACKET_ID::NTF_SYS_CONNECT_SESSION)] = &PacketProcess::NtfSysConnctSession;
		PacketFuncArray[static_cast<int>(PACKET_ID::NTF_SYS_CLOSE_SESSION)] = &PacketProcess::NtfSysCloseSession;
		PacketFuncArray[static_cast<int>(PACKET_ID::LOGIN_REQ)] = &PacketProcess::Login;
		PacketFuncArray[static_cast<int>(PACKET_ID::LOGOUT_REQ)] = &PacketProcess::Logout;
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
		m_Log->Write(LV::INFO, "%s | ConnectSesson. sessionId(%d)", __FUNCTION__, packet.SessionId);

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
				pRoom->NotifyLeaveUserInfo(pUser->GetIndex());
			}

			m_pUserMngr->RemoveUser(packet.SessionId);

			m_Log->Write(LV::INFO, "%s | CloseSesson. User logout %u", __FUNCTION__, pUser->GetIndex());
		}

		m_pConnectedUserManager->SetDisConnectSession(packet.SessionId);

		m_Log->Write(LV::INFO, "%s | CloseSesson. sessionIndex(%d)", __FUNCTION__, packet.SessionId);
		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::DevEcho(PacketInfo packet)
	{
		/* Previous packet struct format code
		auto packetSize = packet.PacketBodySize;

		auto reqPkt = reinterpret_cast<PacketEchoReq*>(packet.pData);

		PacketEchoRes resPkt;
		std::memcpy(&resPkt.Data, reqPkt->Data, packetSize);

		auto packetId = static_cast<short>(PACKET_ID::DEV_ECHO_RES);
		auto data = reinterpret_cast<char*>(&resPkt);
		m_pLogicMain->SendMsg(packet.SessionId, packetId, packetSize, data);
		*/

		lsbProto::Echo echoPkt;
		auto pProto = dynamic_cast<Message*>(&echoPkt);
		ParseDataToProto(pProto, packet.pData, packet.PacketBodySize);

		m_Log->Write(LV::DEBUG, "echo msg : %s", echoPkt.msg().c_str());

		// Echo 이므로... 그대로 다시 전달
		auto packetId = static_cast<short>(PACKET_ID::DEV_ECHO_RES);
		// auto sendSize = static_cast<short>(echoPkt.ByteSize());
		m_pLogicMain->SendProto(packet.SessionId, packetId, pProto);

		return ERROR_CODE::NONE;
	}

	// 받은 데이터 역직렬화 및 파싱
	bool PacketProcess::ParseDataToProto(Message* pProto, char* pData, short size)
	{
		io::ArrayInputStream is(pData, size);
		return pProto->ParseFromZeroCopyStream(&is);
	}
}