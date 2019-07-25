#pragma once

#include <windows.h>

#include <functional>

using packetSizeFunc = std::function<int(char*)>;

class PacketBufferConfig
{
public:
	const int bufferSize;
	const int headerSize;
	const int maxPacketSize;
};

class PacketBufferManager
{
public:
	PacketBufferManager() 
		: m_ReadPos(0)
		, m_WritePos(0)
		, m_BufferSize(0)
		, m_HeaderSize(0)
		, m_MaxPacketSize(0)
		, m_pPacketData(nullptr) {}

	~PacketBufferManager()
	{
		delete[] m_pPacketData;
	}

	bool Init(PacketBufferConfig config)
	{
		if (config.bufferSize < (config.maxPacketSize * 2) || config.headerSize < 1 || config.maxPacketSize < 1)
		{
			return false;
		}

		m_ReadPos = 0;
		m_WritePos = 0;
		m_BufferSize = config.bufferSize;
		m_HeaderSize = config.headerSize;
		m_MaxPacketSize = config.maxPacketSize;
		m_pPacketData = new char[config.bufferSize];

		return true;
	}

	// Write n-bytes to packet buffer
	// and increase write pointer in packet buffer to write data at that time
	bool Write(char* pData, int startIndex, int size)
	{
		if (pData == nullptr)
		{
			return false;
		}

		int remainBufferSize = m_BufferSize - m_WritePos;
		if (remainBufferSize < size)
		{
			return false;
		}

		CopyMemory(m_pPacketData + m_WritePos, pData + startIndex, size);
		m_WritePos += size;

		PreventBufferOverflow();
		return true;
	}

	// Read n-bytes to destination.
	// Before copy data to dest, dest array size will be checked 
	// whether it's size is bigger than packet size being read
	char* Read(char* pDest, int destMaxSize, packetSizeFunc GetPacketSize)
	{
		int readableSize = m_WritePos - m_ReadPos;

		if (readableSize < m_HeaderSize)
		{
			return nullptr;
		}

		// Get the size of entire meaningful packet
		// by calling custom function that server provide
		int packetSize = GetPacketSize(m_pPacketData + m_ReadPos);
		if (readableSize < packetSize || destMaxSize < packetSize)
		{
			return nullptr;
		}

		auto pPacketBody = m_pPacketData + m_ReadPos + m_HeaderSize;
		CopyMemory(pDest, m_pPacketData + m_ReadPos, m_HeaderSize);
		m_ReadPos += packetSize;
		return pPacketBody;
	}

	void IncreseReadPos(int length)
	{
		m_ReadPos += length;
	}

	void IncreseWrtiePos(int length)
	{
		m_WritePos += length;
	}

	char* ReadCurret()
	{
		return m_pPacketData + m_ReadPos;
	}

	char* WriteCurrent()
	{
		return m_pPacketData + m_WritePos;
	}

	int WritableLength()
	{
		return m_BufferSize - m_WritePos;
	}

	int ReablableLength()
	{
		return m_WritePos - m_ReadPos;
	}

	// If write pointer is close to end (i.e., packet buffer is nearly full),
	// pull foward pakcet data on the portion of buffer that alredy read
	void PreventBufferOverflow()
	{
		// Data size that can be read
		int readableSize = m_WritePos - m_ReadPos;
		int WritableSize = m_BufferSize - m_WritePos;
		if (WritableSize < m_MaxPacketSize)
		{
			memmove_s(m_pPacketData, m_BufferSize, m_pPacketData + m_ReadPos, readableSize);
			m_ReadPos = 0;
			m_WritePos = readableSize;
		}
	}

protected:
	char*	m_pPacketData;
	int		m_BufferSize;
	int		m_ReadPos = 0;
	int		m_WritePos = 0;

	int		m_HeaderSize = 0;
	int		m_MaxPacketSize = 0;
};

// Function for getting size of packet in header
// Because packet and header structure is decided by user,
// the way getting packet size should be decided by user too.
// ---
// Example of GetPacketSize function
// when using first 4-bytes int of packet as entire packet size
/*
int GetPakcetSize(char* packet)
{
	int size = *BytesToType<int>(packet);
	return size;
}
*/

// Bit converter
template<typename T>
T* BytesToType(char* const pBytes, const int index = 0)
{
	return reinterpret_cast<T*>(pBytes + index);
}

template<typename T>
char* TypeToBytes(T* const pData)
{
	return reinterpret_cast<char*>(pData);
}