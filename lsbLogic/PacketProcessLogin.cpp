#include <tuple>

#include "Common/ErrorCode.h"
#include "Common/Packet.h"
#include "ConnectedUserManager.h"
#include "User.h"
#include "UserManager.h"
#include "RoomManager.h"
#include "PacketProcess.h"

namespace lsbLogic
{
	ERROR_CODE PacketProcess::Login(PacketInfo packet)
	{
		/* Previous packet struct format code
		PacketLoginRes resPkt;
		auto reqPkt = reinterpret_cast<PacketLoginReq*>(packet.pData);
		
		auto err = m_pUserMngr->AddUser(packet.SessionId, reqPkt->szID);

		auto packetId = static_cast<short>(PACKET_ID::LOGIN_RES);
		auto data = reinterpret_cast<char*>(&resPkt);

		if (err != ERROR_CODE::NONE)
		{
			resPkt.SetErrorCode(err);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sizeof(resPkt), data, nullptr);
			return err;
		}

		m_pConnectedUserManager->SetLogin(packet.SessionId);
		
		m_Log->Write(LV::DEBUG, "%s | Login. id : %s, session Id : %d", __FUNCTION__, reqPkt->szID, packet.SessionId);
		resPkt.SetErrorCode(err);
		m_pLogicMain->SendMsg(packet.SessionId, packetId, sizeof(resPkt), data, nullptr);
		*/

		lsbProto::LoginReq reqPkt;
		auto pReqProto = dynamic_cast<Message*>(&reqPkt);
		ParseDataToProto(pReqProto, packet.pData, packet.PacketBodySize);

		auto err = m_pUserMngr->AddUser(packet.SessionId, reqPkt.id().c_str());

		lsbProto::LoginRes resPkt;
		resPkt.set_res(static_cast<google::protobuf::int32>(err));

		auto packetId = static_cast<short>(PACKET_ID::LOGIN_RES);
		auto pResProto = dynamic_cast<Message*>(&resPkt);

		if (err != ERROR_CODE::NONE)
		{
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
			return err;
		}

		m_pConnectedUserManager->SetLogin(packet.SessionId);

		m_Log->Write(LV::DEBUG, "%s | Login. id : %s, session Id : %d", __FUNCTION__, reqPkt.id().c_str(), packet.SessionId);
		m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);

		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::Logout(PacketInfo packet)
	{
		/* Previous packet struct format code
		PacketLogoutRes resPkt;

		auto packetId = static_cast<short>(PACKET_ID::LOGOUT_RES);
		auto data = reinterpret_cast<char*>(&resPkt);

		auto [err, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (err != ERROR_CODE::NONE)
		{
			resPkt.SetErrorCode(err);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sizeof(resPkt), data, nullptr);
			return err;
		}

		auto pRoom = std::get<1>(m_pRoomMngr->GetRoom(pUser->GetRoomIndex()));

		if (pRoom)
		{
			pRoom->LeaveUser(pUser->GetIndex());
			pRoom->NotifyLeaveUserInfo(pUser->GetIndex());

			m_Log->Write(LOG_LEVEL::DEBUG, "%s | Logout. sessionIndex(%d). Room Out", __FUNCTION__, packet.SessionId);
		}

		m_pUserMngr->RemoveUser(packet.SessionId);

		m_pConnectedUserManager->SetLogout(packet.SessionId);

		m_Log->Write(LV::DEBUG, "%s | Logout. session id : %d", __FUNCTION__, packet.SessionId);
		resPkt.SetErrorCode(err);
		m_pLogicMain->SendMsg(packet.SessionId, packetId, sizeof(resPkt), data, nullptr);
		*/

		lsbProto::LogoutRes resPkt;

		auto packetId = static_cast<short>(PACKET_ID::LOGOUT_RES);
		auto pResProto = dynamic_cast<Message*>(&resPkt);

		auto [err, pUser] = m_pUserMngr->GetUser(packet.SessionId);
		resPkt.set_res(static_cast<google::protobuf::int32>(err));

		if (err != ERROR_CODE::NONE)
		{
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
			return err;
		}

		auto pRoom = std::get<1>(m_pRoomMngr->GetRoom(pUser->GetRoomIndex()));

		if (pRoom)
		{
			pRoom->LeaveUser(pUser->GetIndex());
			pRoom->NotifyLeaveUserInfo(pUser->GetIndex());
		}

		m_pUserMngr->RemoveUser(packet.SessionId);

		m_pConnectedUserManager->SetLogout(packet.SessionId);

		m_Log->Write(LV::INFO, "%s | User logout . session id : %d", __FUNCTION__, packet.SessionId);
		m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);

		return ERROR_CODE::NONE;
	}
}