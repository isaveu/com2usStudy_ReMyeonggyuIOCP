#pragma once

#include <deque>
#include <unordered_map>
#include <string>
#include <memory>

#include "../../Lib_NetworkIOCP/Session.h"
#include "../../Lib_NetworkIOCP/Log.h"
#include "Common/Packet.h"
#include "Common/ErrorCode.h"
#include "User.h"

#pragma warning( push )
#pragma warning( disable : 4125 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 6387 )
#include "Protobuf/Packet.pb.h"
#include <google/protobuf/message.h>
#pragma warning( pop )

using namespace google::protobuf;
using ERROR_CODE = lsbLogic::ERROR_CODE;

namespace lsbLogic
{
	class LogicMain;

	class Room
	{
	public:
		void Init(const short index, const short maxUserCount);

		void SetHandler(LogicMain* pLogicMain, Log* pLogger);

		void Clear();

		short GetIndex();
		bool IsUsed();
		short MaxUserCount();
		short GetUserCount();
		// const wchar_t* GetTitle();
		const std::string& GetTitle();

		// ERROR_CODE CreateRoom(const wchar_t* pRoomTitle);
		ERROR_CODE CreateRoom(const std::string& pRoomTitle);
		// ERROR_CODE EnterUser(User* pUser);
		ERROR_CODE EnterUser(const short index, const int sessionId, const std::string& id);
		ERROR_CODE LeaveUser(const short userIndex);

		void SendProtoToAllUser(const short packetId, Message* pProto, const int passUserIndex);
		// void SendToAllUser(const short packetId, const short dataSize, char* pData, const int passUserindex);
		void NotifyUserList(const int sessionIndex);
		void NotifyEnterUserInfo(const short userIndex, const char* pszUserID);
		void NotifyLeaveUserInfo(const short userIndex);
		// void NotifyChat(const short userIndex, const wchar_t* pszMsg, const short msgLength);
		void NotifyChat(const short userIndex, const std::string& pszMsg);

	private:
		class RoomUser
		{
		public:
			RoomUser() = default;
			void Clear()
			{
				m_Index = -1;
				m_SessionId = -1;
				m_Id = "";
			}
			void Assign(const short index, const int sessionId, const std::string& id)
			{
				m_Index = index;
				m_SessionId = sessionId;
				m_Id = id;
			}

			short			m_Index = -1;
			int				m_SessionId = -1;
			std::string		m_Id;
		};

	private:
		Log* m_pLog = nullptr;

		short m_Index = -1;
		short m_MaxUserCount = 0;

		bool m_IsUsed = false;
		// std::wstring m_Title;
		std::string m_Title;
		// std::unordered_map<short, User*> m_UserList;

		std::deque<short> m_RoomUserIdPool;
		std::vector<RoomUser> m_RoomUserList;

		LogicMain* m_pLogicMain = nullptr;
	};
}