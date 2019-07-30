#pragma once

#include <Ws2tcpip.h>

#include <thread>
#include <string>
#include <sstream>

#include "Log.h"
#include "Thread.h"
#include "ErrorCode.h"

// ws2_32.lib
// https://m.blog.naver.com/PostView.nhn?blogId=gimjoonbum&logNo=220065601788&proxyReferer=https%3A%2F%2Fwww.google.com%2F
#pragma comment(lib, "ws2_32.lib")

class AsyncIONetwork;

class Acceptor : public Thread
{
public:
	Acceptor() = delete;
	Acceptor(
		AsyncIONetwork* const pServer
		, const char* ip
		, const short port
		, Log* const pLog);

	// TODO: define destructor
	~Acceptor() = default;
	void Run() override;

private:
	void Accept();

private:
	// Server to process from client socket
	AsyncIONetwork*	m_pServer;
	
	// Server listen socket
	SOCKET			m_listenSocket;
	
	const char*		m_ip;
	const short		m_port;
	
	// Declared to load winsock.dll only once in run-time
	static bool		m_winsock_dll;

	// Logger (singleton)
	Log*			m_pLogger;
};