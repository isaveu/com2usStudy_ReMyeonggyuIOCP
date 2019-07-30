#include <iostream>

#include "../Lib_NetworkIOCP/AsyncIONetwork.h"
#include "EchoReceiver.h"

int main()
{
	constexpr int threadNumber = 2;
	constexpr int sessionNumber = 1000;

	constexpr int bufferSize = 1024;
	constexpr int headerSize = 0;
	constexpr int maxPacketSize = 200;

	const char* ip = "127.0.0.1";
	constexpr short port = 23452;
	const std::string name = "Echo_Server";

	NetworkConfig config = {
		threadNumber,
		sessionNumber,
		bufferSize,
		headerSize,
		maxPacketSize,
		ip, port, name
	};

	EchoReceiver recv;
	AsyncIONetwork echoServer(&recv, config);
	recv.Init(&echoServer);

	echoServer.Start();

	echoServer.Join();

	return 0;
}