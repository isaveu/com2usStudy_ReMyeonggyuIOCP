#include "../lsbIOCP/Acceptor.h"

class pt {
public:
	pt() { std::cout << "hi" << std::endl; }
	~pt() { std::cout << "bye" << std::endl; }
};

class test {
public:
	std::map<int, std::shared_ptr<pt>> m;
};

int main()
{
	const char* ip = "127.0.0.1";
	const unsigned short port = 11011;
	std::string serverName = "simpleEchoServer";

	auto fileName = utils::Format("Server_port-%d", port);
	Log::GetInstance()->Init(LOG_LEVEL::DEBUG, fileName);


	Acceptor server(ip, port);

	server.Start();

	server.Join();

	return 0;
}