#pragma once

#include <winsock2.h>

#include <atomic>
#include <mutex>
#include <string>

#include "PacketBufferManager.h"

struct ServerConfig
{
	INT threadNumber;

	INT sessionNumber;
	INT ioMaxSize;

	int bufferSize;
	int headerSize;
	int maxPacketSize;

	const char* ip;
	u_short port;
	std::string name;
};


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

public:
	OVERLAPPED		overlapped;
	PacketBufferManager	bufferMngr;
	OP_TYPE			type = OP_TYPE::RECV;
	// INT				length = 0;			// Socket buffer max length // TODO: 없어도 괜찮지 않나?
	INT				requesterId = -1;		// [요청자가 건네는 구분값] for connectSocket()
};

/***********************************************************************************************************/

class IServerController;
// Session descriptor (used in receiver)
// Users manipluate only this, when they need session information
class SESSIONDESC
{
public:
	SESSIONDESC() 
	{ 
		ZeroMemory(&tick, sizeof(tick));
		m_pPacketBuffer = new PacketBufferManager();
	};
	SESSIONDESC(IServerController* pCrtl) : pController(pCrtl) 
	{
		ZeroMemory(&tick, sizeof(tick));
		m_pPacketBuffer = new PacketBufferManager();
	}

	bool CheckTick(SESSIONDESC& desc) const 
	{
		return this->tick.QuadPart == desc.tick.QuadPart; 
	}

public:
	INT				id = -1;
	LARGE_INTEGER	tick;
	IServerController* pController = nullptr;

	PacketBufferManager* m_pPacketBuffer;
};