#include "UserManager.h"

namespace lsbLogic
{
	void UserManager::Init(const short maxUserNumber)
	{
		for (short i = 0; i < maxUserNumber; i++)
		{
			User user{ i };
			m_UserPool.push_back(user);
			m_UserPoolIndex.push_back(i);
		}
	}

	User* UserManager::RetreiveUser()
	{
		if (m_UserPoolIndex.empty())
		{
			return nullptr;
		}

		int index = m_UserPoolIndex.front();
		m_UserPoolIndex.pop_front();

		auto& user = m_UserPool[index];
		user.Clear();

		return &user;
	}

	void UserManager::ReleaseUser(const short index)
	{
		m_UserPoolIndex.push_back(index);
	}

	ERROR_CODE UserManager::AddUser(const int sessionId, const char* id)
	{
		auto idString = std::string(id);
		if (m_IdUserDic.find(idString) != m_IdUserDic.end())
		{
			return ERROR_CODE::USER_MGR_ID_DUPLICATION;
		}

		auto pUser = RetreiveUser();
		if (pUser == nullptr)
		{
			return ERROR_CODE::USER_MGR_MAX_USER_COUNT;
		}

		pUser->Set(sessionId, id);
		m_SessionUserDic.insert({ sessionId, pUser });
		m_IdUserDic.insert({ idString, pUser });

		return ERROR_CODE::NONE;
	}

	ERROR_CODE UserManager::RemoveUser(const int sessionId)
	{
		auto pUser = FindUser(sessionId);

		if (pUser == nullptr)
		{
			return ERROR_CODE::USER_MGR_REMOVE_INVALID_SESSION;
		}

		ReleaseUser(pUser->GetIndex());
		m_IdUserDic.erase(pUser->GetId());
		pUser->Clear();
		m_SessionUserDic.erase(sessionId);

		return ERROR_CODE::NONE;
	}

	User* UserManager::FindUser(const int sessionId) const
	{
		auto iter = m_SessionUserDic.find(sessionId);

		if (iter == m_SessionUserDic.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	std::tuple<ERROR_CODE, User*> UserManager::GetUser(const int sessionId) const
	{
		auto pUser = FindUser(sessionId);

		if (pUser == nullptr)
		{
			return { ERROR_CODE::USER_MGR_INVALID_SESSION_INDEX, nullptr };
		}

		if (pUser->IsCurStateNone())
		{
			return { ERROR_CODE::USER_MGR_NOT_CONFIRM_USER, nullptr };
		}

		return { ERROR_CODE::NONE, pUser };
	}
}