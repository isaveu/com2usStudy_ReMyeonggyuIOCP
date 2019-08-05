#include <iostream>
#include <thread>

#include "../Lib_Logic/LogicMain.h"
#include "flags.h"

lsbLogic::LogicConfig GetConfigFromArgs(const flags::args& args)
{
	const auto threadInput = args.get<int>("thread");
	if (!threadInput) {
		std::cout << "No thread number provided. Default : 2." << std::endl;
	}
	const int threadNumber = threadInput ? *threadInput : 2;

	const auto sessionInput = args.get<int>("session");
	if (!sessionInput) {
		std::cout << "No session number provided. Default : 1,000." << std::endl;
	}
	const int sessionNumber = sessionInput ? *sessionInput : 1000;

	const auto bufferInput = args.get<int>("buffer_size");
	if (!bufferInput) {
		std::cout << "No buffer size provided. Default : 1,024." << std::endl;
	}
	const int bufferSize = bufferInput ? *bufferInput : 1024;

	const auto ipInput = args.get<string>("ip");
	if (!ipInput) {
		std::cout << "No ip address provided. Default : localhost." << std::endl;
	}
	const std::string ip = ipInput ? *ipInput : "127.0.0.1";

	const auto portInput = args.get<short>("port");
	if (!portInput) {
		std::cout << "No port provided. Default : 23452." << std::endl;
	}
	const short port = portInput ? *portInput : 23452;

	const auto nameInput = args.get<string>("name");
	if (!nameInput) {
		std::cout << "No server name provided. Default : SampleServer." << std::endl;
	}
	const std::string name = nameInput ? *nameInput : "SampleServer";

	const auto maxUserInput = args.get<short>("user");
	if (!maxUserInput) {
		std::cout << "No user max number provided. Default : 800." << std::endl;
	}
	const short maxUserNum = maxUserInput ? *maxUserInput : 800;

	const auto maxRoomInput = args.get<short>("room");
	if (!maxRoomInput) {
		std::cout << "No room max number provided. Default : 100." << std::endl;
	}
	const short maxRoomNum = maxRoomInput ? *maxRoomInput : 100;

	return {
		threadNumber,
		sessionNumber,
		bufferSize,
		ip, port, name,
		maxUserNum,
		maxRoomNum,
	};
}

int main(int argc, char** argv)
{
	const flags::args args(argc, argv);
	auto config = GetConfigFromArgs(args);

	lsbLogic::LogicMain myServer;
	myServer.Init(config);
	myServer.Start();

	// TODO: key 입력 시 종료
	char a;
	while (true)
	{
		std::cin >> a;
		if (a == 'q') break;
	}
	myServer.Stop();

	myServer.Join();

	return 0;
}