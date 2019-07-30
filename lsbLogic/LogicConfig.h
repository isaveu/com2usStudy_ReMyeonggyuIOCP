#pragma once

namespace lsbLogic
{
	struct LogicConfig
	{
		int threadNumber;
		int sessionNumber;

		int bufferSize;

		const char* ip;
		short port;
		std::string name;

		short maxUserNum;
		short maxRoomNum;
	};
}