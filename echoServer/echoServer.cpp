#include "../lsbIOCP/IOCPServerNet.h"
#include "../lsbIOCP/Acceptor.h"

int main()
{
	const char* ip = "127.0.0.1";
	u_short port = 11011;

	Acceptor server(ip, port);

	server.Start();

	server.Join();

	return 0;
}