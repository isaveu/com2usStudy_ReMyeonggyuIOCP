#pragma once

#include <windows.h>
#include <functional>

#include <google/protobuf/message.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "AsyncIOException.h"

using namespace google::protobuf;

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
		if (config.bufferSize < (config.maxPacketSize * 2) 
			|| config.headerSize < 0 
			|| config.maxPacketSize < 1)
		{
			ThrowErrorIf(true, WSAENOBUFS, "Packet Buffer size is not sufficient");
		}

		m_PrevReadPos = 0;
		m_ReadPos = 0;
		m_WritePos = 0;
		m_BufferSize = config.bufferSize;
		m_HeaderSize = config.headerSize;
		m_MaxPacketSize = config.maxPacketSize;
		m_pPacketData = new char[config.bufferSize];

		return true;
	}

	void Clear()
	{
		m_WritePos = 0;
		m_ReadPos = 0;
		m_PrevReadPos = 0;
	}

	// Write n-bytes to packet buffer
	// and increase write pointer in packet buffer to write data at that time
	bool Write(char* pData, Message* pMsg, int startIndex, int size, bool relocate = true)
	{
		// 둘 중 하나만 인자로 받아야 함
		if (((pData == nullptr) ^ (pMsg == nullptr)) == false)
		{
			ThrowErrorIf(true, WSAENOBUFS, "Can not write char* and IProto at the same time");
		}

		if (size == 0)
		{
			return true;
		}

		int remainingBufferSize = m_BufferSize;

		// 데이터가 아직 남아있어서 쓰기 커서만 먼저 앞으로 돌아간 상태
		if (m_WritePos < m_PrevReadPos)
		{
			remainingBufferSize = m_PrevReadPos - m_WritePos;
		}
		else
		{
			remainingBufferSize = m_BufferSize - m_WritePos;
		}

		// 이전 데이터들을 제대로 보냄처리 못하고 있거나, 최대 패킷 크기 이상의 요청이 온 것
		if (remainingBufferSize < size)
		{
			return false;
		}

		if (pData)
		{
			// 일반 char* 타입의 패킷형태이면 메모리 카피
			CopyMemory(m_pPacketData + m_WritePos, pData + startIndex, size);
		}
		else if (pMsg)
		{
			// 프로토콜 버프라면
			io::ArrayOutputStream os(m_pPacketData + m_WritePos, size);
			pMsg->SerializeToZeroCopyStream(&os);
		}
		m_WritePos += size;

		// 커서 위치 재정의 처리 여부
		if (relocate == false)
		{
			return true;
		}

		// 패킷이 끝 가까이 도달해서 커서를 앞으로 옮겨야 함
		// Write할 수 있는 크기가 최대 크기 보다 작으면 커서 위치 초기화
		auto writableSize = m_BufferSize - m_WritePos;
		if (writableSize < m_MaxPacketSize)
		{
			// 처음부터 데이터들을 보내지 못하고 있던 것
			// 연결을 끊기 위해 Write 작업을 실패라고 판단
			if (m_PrevReadPos == 0)
			{
				return false;
			}

			// m_ReadPos, m_PrevReadPos는 항상 m_WrtiePos보다 앞서가지 않고
			// 항상 Write 후 Read, ReadComplete 순으로 호출될 것임

			// 커서를 맨 앞으로 옮긴다.
			m_WritePos = 0;
		}

		return true;
	}

	// Read n-bytes to destination.
	// Before copy data to dest, dest array size will be checked 
	// whether it's size is bigger than packet size being read
	char* Read(int length, bool relocate = true)
	{
		auto readPos = m_pPacketData + m_ReadPos;
		m_ReadPos += length;

		// Write와 마찬가지로, 읽은 후 남은 버퍼 크기가 패킷 최대 크기보다 작으면 커서 위치 초기화
		auto readableSize = m_BufferSize - m_ReadPos;
		if (relocate && readableSize < m_MaxPacketSize)
		{
			m_ReadPos = 0;
		}

		return readPos;
	}

	// 버퍼 데이터를 사용하고 해당 크기를 재사용하기 위한 커서 위치 재정의
	void ReadComplete(int length)
	{
		m_PrevReadPos += length;

		// 마찬가지로 남은 버퍼 크기 검사 후 초기화
		auto readableSize = m_BufferSize - m_PrevReadPos;
		if (readableSize < m_MaxPacketSize)
		{
			m_PrevReadPos = 0;
		}
	}

	void IncreseReadPos(int length)
	{
		m_ReadPos += length;
	}

	bool IncreseWrtiePos(int length)
	{
		m_WritePos += length;
		auto writableSize = m_BufferSize - m_WritePos;
		if (writableSize < m_MaxPacketSize)
		{
			if (m_PrevReadPos == 0)
			{
				return false;
			}
			m_WritePos = 0;
		}
		return true;
	}

	char* WriteCurrent()
	{
		return m_pPacketData + m_WritePos;
	}

	// 한 번에 받을 수 있는 데이터의 최대 크기 = 패킷의 최대 크기
	int MaxWriteLegnth()
	{
		return m_MaxPacketSize;
	}

protected:
	char*	m_pPacketData;
	int		m_BufferSize;
	int		m_PrevReadPos = 0;
	int		m_ReadPos = 0;
	int		m_WritePos = 0;

	int		m_HeaderSize = 0;
	int		m_MaxPacketSize = 0;
};