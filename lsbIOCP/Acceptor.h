#pragma once

#include <Ws2tcpip.h>
#include <windows.h>

#include <thread>
#include <string>
#include <sstream>

#include "Log.h"
#include "Thread.h"
#include "AsyncIOException.h"

// ws2_32.lib
// https://m.blog.naver.com/PostView.nhn?blogId=gimjoonbum&logNo=220065601788&proxyReferer=https%3A%2F%2Fwww.google.com%2F
#pragma comment(lib, "ws2_32.lib")

class Acceptor : public Thread
{
public:
	Acceptor() = delete;
	Acceptor(const char* ip, const u_short port);
	~Acceptor() = default;
	void Run() override;

private:
	void Accept();

private:
	SOCKET			m_listenSocket;
	const char*		m_ip;
	const u_short	m_port;
	bool			m_ws2_32_lib = false;
	Log*			m_Log;
};