#pragma once

#include <deque>
#include <vector>

#include "Room.h"

namespace lsbLogic
{
	class RoomManager
	{
	public:
		void Init(LogicMain* pLogicMain, const short RoomNumber, Log* pLogger);
		Room* RetreiveRoom();
		void ReleaseRoom(const int index);
		Room* FindRoom(const int index);
		std::tuple<ERROR_CODE, Room*> GetRoom(const int index);

	private:
		short m_MaxRoomNumber;
		std::deque<int>			m_RoomIndexPool;
		std::vector<Room>		m_RoomPool;
	};
}
