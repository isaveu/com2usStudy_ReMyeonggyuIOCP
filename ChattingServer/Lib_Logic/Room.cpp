#include "Room.h"

#include "LogicMain.h"

namespace lsbLogic
{
	void Room::Init(const short index, const short maxUserCount)
	{
		m_Index = index;
		m_MaxUserCount = maxUserCount;

		m_RoomUserList.assign(maxUserCount, RoomUser());
		for (short i = 0; i < maxUserCount; i++)
		{
			m_RoomUserIdPool.push_back(i);
		}
	}

	void Room::SetHandler(LogicMain* pLogicMain, Log* pLogger)
	{
		m_pLogicMain = pLogicMain;
		m_pLog = pLogger;
	}

	void Room::Clear()
	{
		m_IsUsed = false;
		// m_Title = L"";
		m_Title = "";
		// m_UserList.clear();
	}

	short Room::GetIndex() 
	{
		return m_Index; 
	}

	bool Room::IsUsed() 
	{
		return m_IsUsed; 
	}

	// const wchar_t* Room::GetTitle() 
	const std::string& Room::GetTitle()
	{
		return m_Title; 
	}

	short Room::MaxUserCount() 
	{
		return m_MaxUserCount; 
	}

	short Room::GetUserCount() 
	{
		// return static_cast<short>(m_UserList.size());
		return m_MaxUserCount - static_cast<short>(m_RoomUserIdPool.size());
	}

	// ERROR_CODE Room::CreateRoom(const wchar_t* pRoomTitle)
	ERROR_CODE Room::CreateRoom(const std::string& pRoomTitle)
	{
		if (m_IsUsed)
		{
			return ERROR_CODE::ROOM_ENTER_CREATE_FAIL;
		}

		m_IsUsed = true;
		m_Title = pRoomTitle;

		return ERROR_CODE::NONE;
	}

	// ERROR_CODE Room::EnterUser(User* pUser)
	ERROR_CODE Room::EnterUser(const short index, const int sessionId, const std::string& id)
	{
		if (m_IsUsed == false)
		{
			return ERROR_CODE::ROOM_ENTER_NOT_CREATED;
		}

		// if (m_UserList.size() == m_MaxUserCount)
		if (m_RoomUserIdPool.empty())
		{
			return ERROR_CODE::ROOM_ENTER_MEMBER_FULL;
		}

		auto it = find_if(m_RoomUserList.begin(), m_RoomUserList.end(),
			[&sessionId](RoomUser& user) { return user.m_SessionId == sessionId; });
		if (it != m_RoomUserList.end())
		{
			return ERROR_CODE::ROOM_ENTER_DUPLICATE_SESSION;
		}

		// m_UserList.insert({ pUser->GetIndex(), pUser });
		auto roomUserId = m_RoomUserIdPool.front();
		m_RoomUserIdPool.pop_front();

		m_RoomUserList.at(roomUserId).Assign(index, sessionId, id);

		return ERROR_CODE::NONE;
	}

	ERROR_CODE Room::LeaveUser(const short userIndex)
	{
		if (m_IsUsed == false)
		{
			return ERROR_CODE::ROOM_ENTER_NOT_CREATED;
		}

		// auto iter = m_UserList.find(userIndex);
		// if (iter == m_UserList.end())
		auto it = find_if(m_RoomUserList.begin(), m_RoomUserList.end(),
			[&userIndex](RoomUser& user) { return user.m_Index == userIndex; });
		if (it == m_RoomUserList.end())
		{
			return ERROR_CODE::ROOM_LEAVE_NOT_MEMBER;
		}

		// m_UserList.erase(iter);
		it->Clear();
		auto RoomUserId = static_cast<short>(it - m_RoomUserList.begin());
		m_RoomUserIdPool.push_back(RoomUserId);


		m_pLog->Write(LOG_LEVEL::DEBUG, "%s | User(%d) leave from room (%d).", __FUNCTION__, userIndex, m_Index);

		// if (m_UserList.empty())
		if (m_RoomUserIdPool.size() == m_MaxUserCount)
		{
			Clear();

			m_pLog->Write(LOG_LEVEL::DEBUG, "%s | Room (%d) is now empty.", __FUNCTION__, userIndex, m_Index);
		}

		return ERROR_CODE::NONE;
	}

	/*
	void Room::SendToAllUser(const short packetId, const short dataSize, char* pData, const int passUserIndex = -1)
	{
		for (auto [index, pUser] : m_UserList)
		{
			if (index == passUserIndex)
			{
				continue;
			}

			m_pLogicMain->SendMsg(pUser->GetSessionId(), packetId, dataSize, pData, nullptr);
		}
	}
	*/

	void Room::SendProtoToAllUser(const short packetId, Message* pProto, const int passUserIndex = -1)
	{
		// for (auto [index, pUser] : m_UserList)
		for (auto& roomUser : m_RoomUserList)
		{
			// if (index == passUserIndex)
			if (roomUser.m_SessionId == -1 || roomUser.m_Index == passUserIndex)
			{
				continue;
			}

			// m_pLogicMain->SendProto(pUser->GetSessionId(), packetId, pProto);
			m_pLogicMain->SendProto(roomUser.m_SessionId, packetId, pProto);
		}
	}

	void Room::NotifyUserList(const int sessionId)
	{
		/* Previous packet struct format code
		PacketUserListRes pkt;
		pkt.UserCount = static_cast<short>(m_UserList.size());
		int i = 0;
		for (auto [index, pUser] : m_UserList)
		{
			pkt.UserList[i].UserIndex = index;
			std::memcpy(pkt.UserList[i].UserId, pUser->GetId().c_str(), MAX_USER_ID_SIZE);
			i++;
		}
		auto packetId = static_cast<short>(PACKET_ID::ROOM_USER_LIST);
		auto pktPtr = reinterpret_cast<char*>(&pkt);

		m_pLogicMain->SendMsg(sessionId, packetId, sizeof(pkt), pktPtr, nullptr);
		*/

		lsbProto::RoomUserListNtf ntfPkt;

		// pkt.UserCount = static_cast<short>(m_UserList.size());
		// for (auto [index, pUser] : m_UserList)
		for (auto& roomUser : m_RoomUserList)
		{
			if (roomUser.m_SessionId == -1)
			{
				continue;
			}

			lsbProto::UserInfo* pUserProto = ntfPkt.add_user();
			pUserProto->set_index(roomUser.m_Index);
			pUserProto->set_id(roomUser.m_Id);
		}

		auto packetId = static_cast<short>(PACKET_ID::ROOM_USER_LIST);
		auto pNtfProto = dynamic_cast<Message*>(&ntfPkt);
		m_pLogicMain->SendProto(sessionId, packetId, pNtfProto);
	}

	void Room::NotifyEnterUserInfo(const short userIndex, const char* pszUserID)
	{
		/* Previous packet struct format code
		PacketRoomEnterNtf pkt;
		pkt.User.UserIndex = static_cast<short>(userIndex);
		strncpy_s(pkt.User.UserId, MAX_USER_ID_SIZE + 1, pszUserID, MAX_USER_ID_SIZE);

		auto packetId = static_cast<short>(PACKET_ID::ROOM_ENTER_NEW_USER_NTF);
		auto pktPtr = reinterpret_cast<char*>(&pkt);
		SendToAllUser(packetId, sizeof(pkt), pktPtr, userIndex);
		*/
		lsbProto::RoomEnterNtf ntfPkt;
		
		ntfPkt.mutable_user()->set_index(userIndex);
		ntfPkt.mutable_user()->set_id(pszUserID);

		auto packetId = static_cast<short>(PACKET_ID::ROOM_ENTER_NEW_USER_NTF);
		auto pNtfProto = dynamic_cast<Message*>(&ntfPkt);
		SendProtoToAllUser(packetId, pNtfProto, userIndex);
	}

	void Room::NotifyLeaveUserInfo(const short userIndex)
	{
		/* Previous packet struct format code
		PacketRoomLeaveNtf pkt;
		pkt.UserIndex = userIndex;

		auto packetId = static_cast<short>(PACKET_ID::ROOM_LEAVE_USER_NTF);
		auto pktPtr = reinterpret_cast<char*>(&pkt);
		SendToAllUser(packetId, sizeof(pkt), pktPtr);
		*/

		lsbProto::RoomLeaveNtf ntfPkt;
		ntfPkt.set_userindex(userIndex);

		auto packetId = static_cast<short>(PACKET_ID::ROOM_LEAVE_USER_NTF);
		auto pNtfProto = dynamic_cast<Message*>(&ntfPkt);
		SendProtoToAllUser(packetId, pNtfProto);
	}

	// void Room::NotifyChat(const short userIndex, const wchar_t* pszMsg, const short msgLength)
	void Room::NotifyChat(const short userIndex, const std::string& pszMsg)
	{
		/* Previous packet struct format code
		PacketRoomChatNtf pkt;
		pkt.UserIndex = userIndex;
		pkt.MsgLength = msgLength;
		wcsncpy_s(pkt.Msg, MAX_ROOM_CHAT_MSG_SIZE + 1, pszMsg, msgLength);
		auto pktSize = sizeof(pkt) - MAX_ROOM_CHAT_MSG_SIZE + msgLength;

		auto packetId = static_cast<short>(PACKET_ID::ROOM_CHAT_NTF);
		auto pktPtr = reinterpret_cast<char*>(&pkt);
		SendToAllUser(packetId, static_cast<short>(pktSize), pktPtr);
		*/
		
		lsbProto::RoomChatNtf ntfPkt;
		ntfPkt.set_userindex(userIndex);
		ntfPkt.set_msg(pszMsg);

		auto packetId = static_cast<short>(PACKET_ID::ROOM_CHAT_NTF);
		auto pNtfProto = dynamic_cast<Message*>(&ntfPkt);
		SendProtoToAllUser(packetId, pNtfProto);
	}
}