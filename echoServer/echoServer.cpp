#include <iostream>
#include <thread>

#include "../lsbLogic/LogicMain.h"

int main()
{
	const INT threadNumber = 2;

	const INT sessionNumber = 1000;
	const INT ioMaxSize = 1024;

	const int bufferSize = 1024;
	const int headerSize = 5;
	const int maxPacketSize = 50;

	const char* const ip = "127.0.0.1";
	const u_short port = 23452;
	const std::string name = "SampleServer";

	ServerConfig config =
	{
		threadNumber,

		sessionNumber,
		ioMaxSize,

		bufferSize,
		headerSize,
		maxPacketSize,

		ip, port, name,
	};

	const int maxUserNum = 700;
	const int maxRoomNum = 100;
	const int maxUserNumInRoom = 10;

	lsbLogic::LogicConfig lconfig =
	{
		maxUserNum,
		maxRoomNum,
		maxUserNumInRoom,
	};

	lsbLogic::LogicMain myServer;
	myServer.Init(config, lconfig);
	myServer.Start();

	std::thread logicThread([&]()
		{
			myServer.Run();
		}
	);

	// TODO: key 입력 시 종료
	char a;
	std::cin >> a;
	myServer.Stop();
	logicThread.join();

	return 0;
}