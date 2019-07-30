#pragma once

#include <string>

namespace lsbLogic
{
	enum class USER_STATE
	{
		NONE = 0,
		LOGIN = 1,
		ROOM = 2,
	};

	class User
	{
	public:
		User(const short index) : m_Index(index) {}

		void Clear()
		{
			m_SessionId = -1;
			m_RoomId = -1;
			m_State = USER_STATE::NONE;
			m_Id = "";
		}

		void Set(const int sessionId, const char* id)
		{
			m_State = USER_STATE::LOGIN;
			m_SessionId = sessionId;
			m_Id = id;
		}

		const short GetIndex() const
		{
			return m_Index;
		}

		const int GetSessionId() const
		{
			return m_SessionId;
		}

		const std::string& GetId() const
		{
			return m_Id;
		}

		const short GetRoomIndex() const
		{
			return m_RoomId;
		}

		void ChangeState(USER_STATE state)
		{
			m_State = state;
		}

		bool IsCurStateLogin() const
		{
			return m_State == USER_STATE::LOGIN;
		}

		bool IsCurStateRoom() const
		{
			return m_State == USER_STATE::ROOM;
		}

		bool IsCurStateNone() const
		{
			return m_State == USER_STATE::NONE;
		}

		void EnterRoom(const short roomIndex)
		{
			m_RoomId = roomIndex;
			m_State = USER_STATE::ROOM;
		}

		void LeaveRoom()
		{
			m_RoomId = -1;
			m_State = USER_STATE::LOGIN;
		}

	private:
		short			m_Index = -1;
		short			m_RoomId = -1;
		int				m_SessionId = -1;
		USER_STATE		m_State = USER_STATE::NONE;
		std::string		m_Id;
	};
}