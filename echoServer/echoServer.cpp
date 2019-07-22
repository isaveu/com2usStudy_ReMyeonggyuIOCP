#include "../lsbIOCP/Acceptor.h"
#include "../lsbIOCP/AsyncIOServer.h"
#include "lsbReceiver.h"
#include "../lsbIOCP/PacketBufferManager.h"

int main()
{
	const char* ip = "127.0.0.1";
	const unsigned short port = 23452;
	std::string serverName = "simpleEchoServer";

	DWORD ioMaxBufferSize = 1024;
	DWORD threadNumber = 2;
	DWORD sessionNumber = 1000;

	// Make custom receiver
	lsbReceiver receiver;

	// Make your server with custom receiver
	AsyncIOServer lsbServer(&receiver, ioMaxBufferSize, threadNumber, sessionNumber, serverName);

	// Apply acceptor to your server
	Acceptor acceptor(&lsbServer, ip, port);

	lsbServer.Start();
	acceptor.Start();

	acceptor.Join();
	lsbServer.Join();

	return 0;
}