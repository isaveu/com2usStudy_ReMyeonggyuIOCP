#include <tuple>

#include "ErrorCode.h"
#include "Packet.h"
#include "ConnectedUserManager.h"
#include "User.h"
#include "UserManager.h"
#include "RoomManager.h"
#include "PacketProcess.h"

namespace lsbLogic
{
	ERROR_CODE PacketProcess::Login(PacketInfo packet)
	{
		PacketLoginRes resPkt;
		auto reqPkt = reinterpret_cast<PacketLoginReq*>(packet.pData);
		auto err = m_pUserMngr->AddUser(packet.SessionId, reqPkt->szID);

		auto packetId = static_cast<short>(PACKET_ID::LOGIN_RES);
		auto data = reinterpret_cast<char*>(&resPkt);

		if (err != ERROR_CODE::NONE)
		{
			resPkt.SetErrorCode(err);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sizeof(resPkt), data);
		}

		m_pConnectedUserManager->SetLogin(packet.SessionId);
		
		resPkt.SetErrorCode(err);
		m_pLogicMain->SendMsg(packet.SessionId, packetId, sizeof(resPkt), data);

		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::Logout(PacketInfo packet)
	{
		PacketLogoutRes resPkt;

		auto packetId = static_cast<short>(PACKET_ID::LOGOUT_RES);
		auto data = reinterpret_cast<char*>(&resPkt);

		auto [err, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (err != ERROR_CODE::NONE)
		{
			resPkt.SetErrorCode(err);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sizeof(resPkt), data);
			return err;
		}

		auto pRoom = std::get<1>(m_pRoomMngr->GetRoom(pUser->GetRoomIndex()));

		if (pRoom)
		{
			pRoom->LeaveUser(pUser->GetIndex());
			pRoom->NotifyLeaveUserInfo(pUser->GetId().c_str());

			m_Log->Write(LOG_LEVEL::DEBUG, "%s | Logout. sessionIndex(%d). Room Out", __FUNCTION__, packet.SessionId);
		}

		m_pUserMngr->RemoveUser(packet.SessionId);

		m_pConnectedUserManager->SetDisConnectSession(packet.SessionId);

		resPkt.SetErrorCode(err);
		m_pLogicMain->SendMsg(packet.SessionId, packetId, sizeof(resPkt), data);

		return ERROR_CODE::NONE;
	}
}