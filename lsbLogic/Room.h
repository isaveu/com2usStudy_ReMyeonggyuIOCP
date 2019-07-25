#pragma once

#include <map>
#include <string>
#include <memory>

#include "../lsbIOCP/Session.h"
#include "../lsbIOCP/Log.h"
#include "Packet.h"
#include "ErrorCode.h"
#include "User.h"

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
		const wchar_t* GetTitle();

		ERROR_CODE CreateRoom(const wchar_t* pRoomTitle);
		ERROR_CODE EnterUser(User* pUser);
		ERROR_CODE LeaveUser(const short userIndex);

		void SendToAllUser(const short packetId, const short dataSize, char* pData, const int passUserindex);
		void NotifyEnterUserInfo(const int userIndex, const char* pszUserID);
		void NotifyLeaveUserInfo(const char* pszUserID);
		void NotifyChat(const int sessionIndex, const char* pszUserID, const wchar_t* pszMsg);

	private:
		Log* m_pLog;

		short m_Index = -1;
		short m_MaxUserCount;

		bool m_IsUsed = false;
		std::wstring m_Title;
		std::map<short, User*> m_UserList;

		LogicMain*	m_pLogicMain;
	};
}