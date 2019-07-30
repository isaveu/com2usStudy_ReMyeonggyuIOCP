#pragma once

#include <tuple>
#include <vector>
#include <deque>
#include <map>

#include "User.h"
#include "Common/ErrorCode.h"

using ERROR_CODE = lsbLogic::ERROR_CODE;

namespace lsbLogic
{
	class UserManager
	{
	public:
		void Init(const short maxUserNumber);
		ERROR_CODE AddUser(const int sessionId, const char* Id);
		ERROR_CODE RemoveUser(const int sessionId);

		std::tuple<ERROR_CODE, User*> GetUser(const int sessionId) const;

	private:
		User* RetreiveUser();
		void ReleaseUser(const short index);
		User* FindUser(const int sessionId) const;

	private:
		std::vector<User>	m_UserPool;
		std::deque<int>		m_UserPoolIndex;
		std::map<int, User*>	m_SessionUserDic;
		std::map<std::string, User*>	m_IdUserDic;
	};
}