#include "Room.h"

#include "LogicMain.h"

namespace lsbLogic
{
	void Room::Init(const short index, const short maxUserCount)
	{
		m_Index = index;
		m_MaxUserCount = maxUserCount;
	}

	void Room::SetHandler(LogicMain* pLogicMain, Log* pLogger)
	{
		m_pLogicMain = pLogicMain;
		m_pLog = pLogger;
	}

	void Room::Clear()
	{
		m_IsUsed = false;
		m_Title = L"";
		m_UserList.clear();
	}

	short Room::GetIndex() 
	{
		return m_Index; 
	}

	bool Room::IsUsed() 
	{
		return m_IsUsed; 
	}

	const wchar_t* Room::GetTitle() 
	{
		return m_Title.c_str(); 
	}

	short Room::MaxUserCount() 
	{
		return m_MaxUserCount; 
	}

	short Room::GetUserCount() 
	{
		return static_cast<short>(m_UserList.size());
	}

	ERROR_CODE Room::CreateRoom(const wchar_t* pRoomTitle)
	{
		if (m_IsUsed)
		{
			return ERROR_CODE::ROOM_ENTER_CREATE_FAIL;
		}

		m_IsUsed = true;
		m_Title = pRoomTitle;

		return ERROR_CODE::NONE;
	}

	ERROR_CODE Room::EnterUser(User* pUser)
	{
		if (m_IsUsed == false)
		{
			return ERROR_CODE::ROOM_ENTER_NOT_CREATED;
		}

		if (m_UserList.size() == m_MaxUserCount)
		{
			return ERROR_CODE::ROOM_ENTER_MEMBER_FULL;
		}

		m_UserList.insert({ pUser->GetIndex(), pUser });
		return ERROR_CODE::NONE;
	}

	ERROR_CODE Room::LeaveUser(const short userIndex)
	{
		if (m_IsUsed == false)
		{
			return ERROR_CODE::ROOM_ENTER_NOT_CREATED;
		}

		auto iter = m_UserList.find(userIndex);
		if (iter == m_UserList.end())
		{
			return ERROR_CODE::ROOM_LEAVE_NOT_MEMBER;
		}

		m_UserList.erase(iter);

		if (m_UserList.empty())
		{
			Clear();
		}

		return ERROR_CODE::NONE;
	}

	void Room::SendToAllUser(const short packetId, const short dataSize, char* pData, const int passUserIndex = -1)
	{
		for (auto [index, pUser] : m_UserList)
		{
			if (index == passUserIndex)
			{
				continue;
			}

			m_pLogicMain->SendMsg(pUser->GetSessionId(), packetId, dataSize, pData);
		}
	}

	void Room::NotifyEnterUserInfo(const int userIndex, const char* pszUserID)
	{
		PacketRoomEnterNtf pkt;
		strncpy_s(pkt.UserId, MAX_USER_ID_SIZE + 1, pszUserID, MAX_USER_ID_SIZE);

		auto packetId = static_cast<short>(PACKET_ID::ROOM_ENTER_NEW_USER_NTF);
		auto pktPtr = reinterpret_cast<char*>(&pkt);
		SendToAllUser(packetId, sizeof(pkt), pktPtr, userIndex);
	}

	void Room::NotifyLeaveUserInfo(const char* pszUserID)
	{
		PacketRoomLeaveNtf pkt;
		strncpy_s(pkt.UserId, MAX_USER_ID_SIZE + 1, pszUserID, MAX_USER_ID_SIZE);

		auto packetId = static_cast<short>(PACKET_ID::ROOM_LEAVE_USER_NTF);
		auto pktPtr = reinterpret_cast<char*>(&pkt);
		SendToAllUser(packetId, sizeof(pkt), pktPtr);
	}

	void Room::NotifyChat(const int userIndex, const char* pszUserID, const wchar_t* pszMsg)
	{
		PacketRoomChatNtf pkt;
		strncpy_s(pkt.UserId, MAX_USER_ID_SIZE + 1, pszUserID, MAX_USER_ID_SIZE);
		wcsncpy_s(pkt.Msg, MAX_ROOM_CHAT_MSG_SIZE + 1, pszMsg, MAX_ROOM_CHAT_MSG_SIZE);

		auto packetId = static_cast<short>(PACKET_ID::ROOM_CHAT_NTF);
		auto pktPtr = reinterpret_cast<char*>(&pkt);
		SendToAllUser(packetId, sizeof(pkt), pktPtr, userIndex);
	}
}