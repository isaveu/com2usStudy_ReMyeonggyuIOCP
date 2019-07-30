#pragma once

#include <map>
#include <string>
#include <memory>

#include "../lsbIOCP/Session.h"
#include "../lsbIOCP/Log.h"
#include "Common/Packet.h"
#include "Common/ErrorCode.h"
#include "User.h"
#include "Protobuf/Packet.pb.h"

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
		ERROR_CODE EnterUser(User* pUser);
		ERROR_CODE LeaveUser(const short userIndex);

		void SendProtoToAllUser(const short packetId, Message* pProto, const int passUserIndex);
		void SendToAllUser(const short packetId, const short dataSize, char* pData, const int passUserindex);
		void NotifyUserList(const int sessionIndex);
		void NotifyEnterUserInfo(const short userIndex, const char* pszUserID);
		void NotifyLeaveUserInfo(const short userIndex);
		// void NotifyChat(const short userIndex, const wchar_t* pszMsg, const short msgLength);
		void NotifyChat(const short userIndex, const std::string& pszMsg);

	private:
		Log* m_pLog = nullptr;

		short m_Index = -1;
		short m_MaxUserCount = 0;

		bool m_IsUsed = false;
		// std::wstring m_Title;
		std::string m_Title;
		std::map<short, User*> m_UserList;

		LogicMain*	m_pLogicMain = nullptr;
	};
}