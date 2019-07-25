#include "RoomManager.h"

namespace lsbLogic
{
	void RoomManager::Init(const short RoomNumber, const short maxUserCount)
	{
		for (short i = 0; i < RoomNumber; i++)
		{
			m_RoomIndexPool.push_back(i);
			Room room;
			room.Init(i, maxUserCount);

			m_RoomPool.push_back(room);
		}
	}

	Room* RoomManager::RetreiveRoom()
	{
		if (m_RoomIndexPool.empty())
		{
			return nullptr;
		}

		auto index = m_RoomIndexPool.front();
		m_RoomIndexPool.pop_front();

		auto& room = m_RoomPool[index];
		room.Clear();

		return &room;
	}

	void RoomManager::ReleaseRoom(const int index)
	{
		m_RoomIndexPool.push_back(index);
	}

	Room* RoomManager::FindRoom(const int index)
	{
		if (m_RoomPool[index].IsUsed() == false)
		{
			return nullptr;
		}

		return &m_RoomPool[index];
	}

	std::tuple<ERROR_CODE, Room*> RoomManager::GetRoom(const int index)
	{
		auto pRoom = FindRoom(index);
		
		if (pRoom == nullptr)
		{
			return { ERROR_CODE::ROOM_ENTER_NOT_CREATED, nullptr };
		}

		return { ERROR_CODE::NONE, nullptr };
	}
}