#include <tuple>

#include "Common/Packet.h"
#include "Common/ErrorCode.h"
#include "Room.h"
#include "RoomManager.h"
#include "User.h"
#include "UserManager.h"
#include "PacketProcess.h"

namespace lsbLogic
{
	ERROR_CODE PacketProcess::RoomEnter(PacketInfo packet)
	{
		/* Previous packet struct format code
		auto reqPkt = (PacketRoomEnterReq*)packet.pData;
		PacketRoomEnterRes resPkt;
		resPkt.RoomIndex = -1;

		auto packetId = static_cast<short>(PACKET_ID::ROOM_ENTER_RES);
		auto sendSize = static_cast<short>(sizeof(resPkt));
		auto data = reinterpret_cast<char*>(&resPkt);

		auto [err, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (err != ERROR_CODE::NONE) {
			resPkt.SetErrorCode(err);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
			return err;
		}

		if (pUser->IsCurStateLogin() == false) {
			resPkt.SetErrorCode(ERROR_CODE::ROOM_ENTER_INVALID_DOMAIN);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
			return ERROR_CODE::ROOM_ENTER_INVALID_DOMAIN;
		}

		Room* pRoom = nullptr;

		if (reqPkt->IsCreate)
		{
			pRoom = m_pRoomMngr->RetreiveRoom();
			if (pRoom == nullptr) {
				resPkt.SetErrorCode(ERROR_CODE::ROOM_ENTER_EMPTY_ROOM);
				m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
				return ERROR_CODE::ROOM_ENTER_EMPTY_ROOM;
			}

			auto ret = pRoom->CreateRoom(reqPkt->RoomTitle);
			if (ret != ERROR_CODE::NONE) {
				resPkt.SetErrorCode(ret);
				m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
				return ret;
			}

			resPkt.RoomIndex = pRoom->GetIndex();
			std::wmemcpy(resPkt.RoomTitle, reqPkt->RoomTitle, MAX_ROOM_TITLE_SIZE);
		}
		else
		{
			pRoom = std::get<1>(m_pRoomMngr->GetRoom(reqPkt->RoomIndex));
			if (pRoom == nullptr) {
				resPkt.SetErrorCode(ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX);
				m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
				return ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
			}

			resPkt.RoomIndex = pRoom->GetIndex();
			std::wmemcpy(resPkt.RoomTitle, pRoom->GetTitle(), MAX_ROOM_TITLE_SIZE);
		}


		auto enterRet = pRoom->EnterUser(pUser);
		if (enterRet != ERROR_CODE::NONE) {
			resPkt.SetErrorCode(enterRet);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
			return enterRet;
		}

		// 유저 정보를 룸에 들어왔다고 변경한다.
		pUser->EnterRoom(pRoom->GetIndex());

		// 룸에 새 유저 들어왔다고 알린다
		pRoom->NotifyEnterUserInfo(pUser->GetIndex(), pUser->GetId().c_str());

		m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
		m_Log->Write(LOG_LEVEL::INFO, "%s | Enter Room. sessionId(%d)", __FUNCTION__, packet.SessionId);

		// send user list
		pRoom->NotifyUserList(packet.SessionId);
		m_Log->Write(LOG_LEVEL::INFO, "%s | Give User List. sessionId(%d)", __FUNCTION__, packet.SessionId);
		*/

		lsbProto::RoomEnterReq reqPkt;
		auto pReqProto = dynamic_cast<Message*>(&reqPkt);
		ParseDataToProto(pReqProto, packet.pData, packet.PacketBodySize);

		lsbProto::RoomEnterRes resPkt;
		auto pResProto = dynamic_cast<Message*>(&resPkt);

		auto packetId = static_cast<short>(PACKET_ID::ROOM_ENTER_RES);

		auto [errUser, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (errUser != ERROR_CODE::NONE) {
			resPkt.set_res(static_cast<google::protobuf::int32>(errUser));
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
			return errUser;
		}

		if (pUser->IsCurStateLogin() == false) {
			auto err = ERROR_CODE::ROOM_ENTER_INVALID_DOMAIN;
			resPkt.set_res(static_cast<google::protobuf::int32>(err));
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
			return err;
		}

		Room* pRoom = nullptr;

		if (reqPkt.iscreate())
		{
			pRoom = m_pRoomMngr->RetreiveRoom();
			if (pRoom == nullptr) {
				auto err = ERROR_CODE::ROOM_ENTER_EMPTY_ROOM; 
				resPkt.set_res(static_cast<google::protobuf::int32>(err));
				m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
				return err;
			}

			auto ret = pRoom->CreateRoom(reqPkt.roomtitle());
			if (ret != ERROR_CODE::NONE) {
				resPkt.set_res(static_cast<google::protobuf::int32>(ret));
				m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
				return ret;
			}

			resPkt.set_roomindex(pRoom->GetIndex());
			resPkt.set_roomtitle(reqPkt.roomtitle());
		}
		else
		{
			pRoom = std::get<1>(m_pRoomMngr->GetRoom(reqPkt.roomindex()));
			if (pRoom == nullptr) {
				auto err = ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
				resPkt.set_res(static_cast<google::protobuf::int32>(err));
				m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
				return err;
			}

			resPkt.set_roomindex(pRoom->GetIndex());
			resPkt.set_roomtitle(pRoom->GetTitle());
		}


		auto enterRet = pRoom->EnterUser(pUser);
		if (enterRet != ERROR_CODE::NONE) {
			resPkt.set_res(static_cast<google::protobuf::int32>(enterRet));
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
			return enterRet;
		}

		// 유저 정보를 룸에 들어왔다고 변경한다.
		pUser->EnterRoom(pRoom->GetIndex());

		// 룸에 새 유저 들어왔다고 알린다
		pRoom->NotifyEnterUserInfo(pUser->GetIndex(), pUser->GetId().c_str());

		m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
		m_Log->Write(LOG_LEVEL::INFO, "%s | Enter Room. sessionId(%d)", __FUNCTION__, packet.SessionId);

		// send user list
		pRoom->NotifyUserList(packet.SessionId);
		m_Log->Write(LOG_LEVEL::INFO, "%s | Give User List. sessionId(%d)", __FUNCTION__, packet.SessionId);

		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::RoomLeave(PacketInfo packet)
	{
		/* Previous packet struct format code
		PacketRoomLeaveRes resPkt;

		auto packetId = static_cast<short>(PACKET_ID::ROOM_LEAVE_RES);
		auto sendSize = static_cast<short>(sizeof(resPkt));
		auto data = reinterpret_cast<char*>(&resPkt);

		auto [errorCode, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (errorCode != ERROR_CODE::NONE) {
			resPkt.SetErrorCode(errorCode);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
			return errorCode;
		}

		auto userIndex = pUser->GetIndex();

		if (pUser->IsCurStateRoom() == false) {
			resPkt.SetErrorCode(ERROR_CODE::ROOM_LEAVE_INVALID_DOMAIN);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
			return ERROR_CODE::ROOM_LEAVE_INVALID_DOMAIN;
		}

		auto [err, pRoom] = m_pRoomMngr->GetRoom(pUser->GetRoomIndex());
		if (pRoom == nullptr) {
			resPkt.SetErrorCode(ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
			return ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
		}

		auto leaveRet = pRoom->LeaveUser(userIndex);
		if (leaveRet != ERROR_CODE::NONE) {
			resPkt.SetErrorCode(leaveRet);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
			return leaveRet;
		}

		pUser->LeaveRoom();

		// 룸에 유저가 나갔음을 통보
		pRoom->NotifyLeaveUserInfo(pUser->GetIndex());

		m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
		m_Log->Write(LOG_LEVEL::INFO, "%s | Leave Room. sessionId(%d)", __FUNCTION__, packet.SessionId);
		*/

		lsbProto::RoomLeaveRes resPkt;

		auto packetId = static_cast<short>(PACKET_ID::ROOM_LEAVE_RES);
		auto pResProto = dynamic_cast<Message*>(&resPkt);

		auto [errorCode, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (errorCode != ERROR_CODE::NONE) {
			resPkt.set_res(static_cast<google::protobuf::int32>(errorCode));
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
			return errorCode;
		}

		auto userIndex = pUser->GetIndex();

		if (pUser->IsCurStateRoom() == false) {
			auto err = ERROR_CODE::ROOM_LEAVE_INVALID_DOMAIN;
			resPkt.set_res(static_cast<google::protobuf::int32>(err));
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
			return err;
		}

		auto [errRoom, pRoom] = m_pRoomMngr->GetRoom(pUser->GetRoomIndex());
		if (errRoom != ERROR_CODE::NONE) {
			auto err = ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
			resPkt.set_res(static_cast<google::protobuf::int32>(err));
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
			return err;
		}

		auto leaveRet = pRoom->LeaveUser(userIndex);
		if (leaveRet != ERROR_CODE::NONE) {
			resPkt.set_res(static_cast<google::protobuf::int32>(leaveRet));
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
			return leaveRet;
		}

		pUser->LeaveRoom();

		// 룸에 유저가 나갔음을 통보
		pRoom->NotifyLeaveUserInfo(pUser->GetIndex());

		m_pLogicMain->SendProto(packet.SessionId, packetId, pResProto);
		m_Log->Write(LOG_LEVEL::INFO, "%s | Leave Room. sessionId(%d)", __FUNCTION__, packet.SessionId);
		return ERROR_CODE::NONE;
	}

	ERROR_CODE PacketProcess::RoomChat(PacketInfo packet)
	{
		/* Previous packet struct format code
		auto reqPkt = reinterpret_cast<PacketRoomChatReq*>(packet.pData);
		PacketRoomChatRes resPkt;

		auto packetId = static_cast<short>(PACKET_ID::ROOM_CHAT_RES);
		auto sendSize = static_cast<short>(sizeof(resPkt));
		auto data = reinterpret_cast<char*>(&resPkt);

		auto [errorCode, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (errorCode != ERROR_CODE::NONE) {
			resPkt.SetErrorCode(errorCode);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
			return errorCode;
		}

		if (pUser->IsCurStateRoom() == false) {
			resPkt.SetErrorCode(ERROR_CODE::ROOM_CHAT_INVALID_DOMAIN);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
			return ERROR_CODE::ROOM_CHAT_INVALID_DOMAIN;
		}

		auto [err, pRoom] = m_pRoomMngr->GetRoom(pUser->GetRoomIndex());
		if (pRoom == nullptr) {
			resPkt.SetErrorCode(ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX);
			m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
			return ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
		}
		auto MsgLength = reqPkt->MsgLength;
		pRoom->NotifyChat(pUser->GetIndex(), reqPkt->Msg, MsgLength);

		m_pLogicMain->SendMsg(packet.SessionId, packetId, sendSize, data, nullptr);
		*/

		lsbProto::RoomChatReq reqPkt;
		auto pReqPkt = dynamic_cast<Message*>(&reqPkt);
		ParseDataToProto(pReqPkt, packet.pData, packet.PacketBodySize);

		lsbProto::RoomChatRes resPkt;

		auto packetId = static_cast<short>(PACKET_ID::ROOM_CHAT_RES);
		auto pResPkt = dynamic_cast<Message*>(&resPkt);

		auto [errUser, pUser] = m_pUserMngr->GetUser(packet.SessionId);

		if (errUser != ERROR_CODE::NONE) {
			resPkt.set_res(static_cast<google::protobuf::int32>(errUser));
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResPkt);
			return errUser;
		}

		if (pUser->IsCurStateRoom() == false) {
			auto err = ERROR_CODE::ROOM_CHAT_INVALID_DOMAIN;
			resPkt.set_res(static_cast<google::protobuf::int32>(err));
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResPkt);
			return err;
		}

		auto [errRoom, pRoom] = m_pRoomMngr->GetRoom(pUser->GetRoomIndex());
		if (errRoom != ERROR_CODE::NONE) {
			auto err = ERROR_CODE::ROOM_ENTER_INVALID_ROOM_INDEX;
			resPkt.set_res(static_cast<google::protobuf::int32>(err));
			m_pLogicMain->SendProto(packet.SessionId, packetId, pResPkt);
			return err;
		}

		pRoom->NotifyChat(pUser->GetIndex(), reqPkt.msg());

		m_pLogicMain->SendProto(packet.SessionId, packetId, pResPkt);
		return ERROR_CODE::NONE;
	}
}