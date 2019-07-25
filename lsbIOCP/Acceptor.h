#pragma once

#include <Ws2tcpip.h>

#include <thread>
#include <string>
#include <sstream>

#include "Log.h"
#include "Thread.h"
#include "AsyncIOException.h"

// ws2_32.lib
// https://m.blog.naver.com/PostView.nhn?blogId=gimjoonbum&logNo=220065601788&proxyReferer=https%3A%2F%2Fwww.google.com%2F
#pragma comment(lib, "ws2_32.lib")

class AsyncIOServer;

class Acceptor : public Thread
{
public:
	Acceptor() = delete;
	Acceptor(AsyncIOServer* pServer, 
		const char* ip, 
		const u_short port, 
		Log* const pLog);
	~Acceptor() = default;
	void Run() override;

private:
	void Accept();

private:
	// Server to process from client socket
	AsyncIOServer*	m_pServer;
	
	// Server listen socket
	SOCKET			m_listenSocket;
	
	const char*		m_ip;
	const u_short	m_port;
	
	// Declared to load winsock.dll only once in run-time
	static bool		m_winsock_dll;

	// Logger (singleton)
	Log*			m_Log;
};