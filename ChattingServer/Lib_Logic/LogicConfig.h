#pragma once

namespace lsbLogic
{
	struct LogicConfig
	{
		int threadNumber;
		int sessionNumber;

		int bufferSize;

		std::string ip;
		short port;
		std::string name;

		short maxUserNum;
		short maxRoomNum;
	};
}