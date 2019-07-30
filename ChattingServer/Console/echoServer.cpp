#include <iostream>
#include <thread>

#include "../Lib_Logic/LogicMain.h"

int main()
{
	constexpr int threadNumber = 2;
	constexpr int sessionNumber = 1000;
	constexpr int bufferSize = 1024;

	const char* const ip = "127.0.0.1";
	const short port = 23452;
	const std::string name = "SampleServer";

	const short maxUserNum = 800;
	const short maxRoomNum = 100;

	lsbLogic::LogicConfig config =
	{
		threadNumber,
		sessionNumber,
		bufferSize,
		ip, port, name,
		maxUserNum,
		maxRoomNum,
	};

	lsbLogic::LogicMain myServer;
	myServer.Init(config);
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