#include "RoomManager.h"

namespace lsbLogic
{
	void RoomManager::Init(LogicMain* pLogicMain, const short RoomNumber, Log* pLogger)
	{
		m_MaxRoomNumber = RoomNumber;
		for (short i = 0; i < RoomNumber; i++)
		{
			m_RoomIndexPool.push_back(i);
			Room room;
			// TODO: 방당 유저 수 config에서 받기 
			room.Init(i, MAX_ROOM_USER_COUNT);
			room.SetHandler(pLogicMain, pLogger);

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
		if (index < 0 || index >= m_MaxRoomNumber)
		{
			return nullptr;
		}

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

		return { ERROR_CODE::NONE, pRoom };
	}
}