#pragma once

#include <winsock2.h>

#include <atomic>
#include <mutex>
#include <string>

#include "PacketBufferManager.h"

struct NetworkConfig
{
	int threadNumber;
	int sessionNumber;

	int bufferSize;
	int headerSize;
	int maxPacketSize;

	const char* ip;
	short port;
	std::string name;
};

/***********************************************************************************************************/

enum OP_TYPE { RECV, SEND, CONN };

// Overlapped extended struct
class OVERLAPPED_EX
{
public:
	OVERLAPPED_EX() 
	{
		ZeroMemory(&overlapped, sizeof(overlapped));
		type = OP_TYPE::CONN;
	}

	OVERLAPPED_EX(PacketBufferConfig pktBufferConfig, OP_TYPE _type) : type(_type)
	{
		ZeroMemory(&overlapped, sizeof(overlapped));
		bufferMngr.Init(pktBufferConfig);
	}

	void Clear()
	{
		requesterId = -1;
		bufferMngr.Clear();
	}

public:
	OVERLAPPED		overlapped;
	PacketBufferManager	bufferMngr;
	OP_TYPE			type = OP_TYPE::RECV;
	// INT				length = 0;			// Socket buffer max length // TODO: 없어도 괜찮지 않나?
	INT				requesterId = -1;		// [요청자가 건네는 구분값] for connectSocket()
};

/***********************************************************************************************************/