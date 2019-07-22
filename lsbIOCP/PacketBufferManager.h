#pragma once

#include <windows.h>

#include <functional>

using packetSizeFunc = std::function<int(char*)>;

class PacketBufferManager
{
public:
	PacketBufferManager(packetSizeFunc function) 
		: GetPacketSize(function)
		, m_ReadPos(0)
		, m_WritePos(0)
		, m_BufferSize(0)
		, m_HeaderSize(0)
		, m_MaxPacketSize(0)
		, m_pPacketData(nullptr) {}

	~PacketBufferManager()
	{
		delete[] m_pPacketData;
	}

	bool Init(int size, int headerSize, int maxPacketSize)
	{
		if (size < (maxPacketSize * 2) || size < 1 || headerSize < 1 || maxPacketSize < 1)
			return false;

		m_ReadPos = 0;
		m_WritePos = 0;
		m_BufferSize = size;
		m_HeaderSize = headerSize;
		m_MaxPacketSize = maxPacketSize;
		m_pPacketData = new char[size];

		return true;
	}

	// Write n-bytes to packet buffer
	// and increase write pointer in packet buffer to write data at that time
	bool Write(char* data, int startIndex, int size)
	{
		if (data == nullptr) return false;

		int remainBufferSize = m_BufferSize - m_WritePos;
		if (remainBufferSize < size) return false;

		CopyMemory(m_pPacketData + m_WritePos, data + startIndex, size);
		m_WritePos += size;

		PreventBufferOverflow();
	}

	// Read n-bytes to destination.
	// Before copy data to dest, dest array size will be checked 
	// whether it's size is bigger than packet size being read
	bool Read(char* dest, int destMaxSize)
	{
		int readableSize = m_WritePos - m_ReadPos;

		if (readableSize < m_HeaderSize) return false;

		// Get the size of entire meaningful packet
		// by calling custom function that server provide
		int packetSize = GetPacketSize(m_pPacketData + m_ReadPos);
		if (readableSize < packetSize) return false;
		if (destMaxSize < packetSize) return false;

		CopyMemory(dest, m_pPacketData + m_ReadPos, packetSize);
		m_ReadPos += packetSize;
		return true;
	}

private:
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
	
	// Function for getting size of packet in header
	// Because packet and header structure is decided by libaray user,
	// the way getting packet size should be decided by user too.
	packetSizeFunc GetPacketSize;
};

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
T* BytesToType(char* bytes, int index = 0)
{
	return reinterpret_cast<T*>(bytes + index);
}

template<typename T>
char* TypeToBytes(T* data)
{
	return reinterpret_cast<char*>(data);
}