#include <tuple>

#include "Packet.h"
#include "ErrorCode.h"
#include "Room.h"
#include "RoomManager.h"
#include "User.h"
#include "UserManager.h"
#include "PacketProcess.h"

namespace lsbLogic
{
	ERROR_CODE PacketProcess::RoomEnter(PacketInfo packet)
	{
		auto reqPkt = (PacketRoomEnterReq*)packet.pData;
		PacketRoomEnterRes resPkt;

		auto packetId = static_cast<short>(PACKET_ID::ROOM_ENTER_RES);
		auto sendSize = static_cast<short>(sizeof(resPkt));
		auto data = reinterpret_cast<char*>(&resPkt);

		auto [err, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (err != ERROR_CODE::NONE) {
			resPkt.SetErrorCode(err);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
			return err;
		}

		if (pUser->IsCurStateLogin() == false) {
			resPkt.SetErrorCode(ERROR_CODE::ROOM_ENTER_INVALID_DOMAIN);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
			return ERROR_CODE::ROOM_ENTER_INVALID_DOMAIN;
		}

		Room* pRoom = nullptr;

		if (reqPkt->IsCreate)
		{
			pRoom = m_pRoomMngr->RetreiveRoom();
			if (pRoom == nullptr) {
				resPkt.SetErrorCode(ERROR_CODE::ROOM_ENTER_EMPTY_ROOM);
				m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
				return ERROR_CODE::ROOM_ENTER_EMPTY_ROOM;
			}

			auto ret = pRoom->CreateRoom(reqPkt->RoomTitle);
			if (ret != ERROR_CODE::NONE) {
				resPkt.SetErrorCode(ret);
				m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
				return ret;
			}
		}
		else
		{
			pRoom = std::get<1>(m_pRoomMngr->GetRoom(reqPkt->RoomIndex));
			if (pRoom == nullptr) {
				resPkt.SetErrorCode(ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX);
				m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
				return ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
			}
		}


		auto enterRet = pRoom->EnterUser(pUser);
		if (enterRet != ERROR_CODE::NONE) {
			resPkt.SetErrorCode(enterRet);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
			return enterRet;
		}

		// 유저 정보를 룸에 들어왔다고 변경한다.
		pUser->EnterRoom(pRoom->GetIndex());

		// 룸에 새 유저 들어왔다고 알린다
		pRoom->NotifyEnterUserInfo(pUser->GetIndex(), pUser->GetId().c_str());

		m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::RoomLeave(PacketInfo packet)
	{
		PacketRoomLeaveRes resPkt;

		auto packetId = static_cast<short>(PACKET_ID::ROOM_LEAVE_RES);
		auto sendSize = static_cast<short>(sizeof(resPkt));
		auto data = reinterpret_cast<char*>(&resPkt);

		auto [errorCode, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (errorCode != ERROR_CODE::NONE) {
			resPkt.SetErrorCode(errorCode);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
			return errorCode;
		}

		auto userIndex = pUser->GetIndex();

		if (pUser->IsCurStateRoom() == false) {
			resPkt.SetErrorCode(ERROR_CODE::ROOM_LEAVE_INVALID_DOMAIN);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
			return ERROR_CODE::ROOM_LEAVE_INVALID_DOMAIN;
		}

		auto [err, pRoom] = m_pRoomMngr->GetRoom(pUser->GetRoomIndex());
		if (pRoom == nullptr) {
			resPkt.SetErrorCode(ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
			return ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
		}

		auto leaveRet = pRoom->LeaveUser(userIndex);
		if (leaveRet != ERROR_CODE::NONE) {
			resPkt.SetErrorCode(leaveRet);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
			return leaveRet;
		}

		pUser->LeaveRoom();

		// 룸에 유저가 나갔음을 통보
		pRoom->NotifyLeaveUserInfo(pUser->GetId().c_str());

		m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::RoomChat(PacketInfo packet)
	{
		auto reqPkt = reinterpret_cast<PacketRoomChatReq*>(packet.pData);
		PacketRoomChatRes resPkt;

		auto packetId = static_cast<short>(PACKET_ID::ROOM_CHAT_RES);
		auto sendSize = static_cast<short>(sizeof(resPkt));
		auto data = reinterpret_cast<char*>(&resPkt);

		auto [errorCode, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (errorCode != ERROR_CODE::NONE) {
			resPkt.SetErrorCode(errorCode);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
			return errorCode;
		}

		if (pUser->IsCurStateRoom() == false) {
			resPkt.SetErrorCode(ERROR_CODE::ROOM_CHAT_INVALID_DOMAIN);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
			return ERROR_CODE::ROOM_CHAT_INVALID_DOMAIN;
		}

		auto [err, pRoom] = m_pRoomMngr->GetRoom(pUser->GetRoomIndex());
		if (pRoom == nullptr) {
			resPkt.SetErrorCode(ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
			return ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
		}

		pRoom->NotifyChat(pUser->GetSessionId(), pUser->GetId().c_str(), reqPkt->Msg);

		m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data);
		return ERROR_CODE::NONE;
	}
}