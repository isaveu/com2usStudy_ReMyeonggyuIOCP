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
		// �� �� �ϳ��� ���ڷ� �޾ƾ� ��
		if (((pData == nullptr) ^ (pMsg == nullptr)) == false)
		{
			ThrowErrorIf(true, WSAENOBUFS, "Can not write char* and IProto at the same time");
		}

		if (size == 0)
		{
			return true;
		}

		int remainingBufferSize = m_BufferSize;

		// �����Ͱ� ���� �����־ ���� Ŀ���� ���� ������ ���ư� ����
		if (m_WritePos < m_PrevReadPos)
		{
			remainingBufferSize = m_PrevReadPos - m_WritePos;
		}
		else
		{
			remainingBufferSize = m_BufferSize - m_WritePos;
		}

		// ���� �����͵��� ����� ����ó�� ���ϰ� �ְų�, �ִ� ��Ŷ ũ�� �̻��� ��û�� �� ��
		if (remainingBufferSize < size)
		{
			return false;
		}

		if (pData)
		{
			// �Ϲ� char* Ÿ���� ��Ŷ�����̸� �޸� ī��
			CopyMemory(m_pPacketData + m_WritePos, pData + startIndex, size);
		}
		else if (pMsg)
		{
			// �������� �������
			io::ArrayOutputStream os(m_pPacketData + m_WritePos, size);
			pMsg->SerializeToZeroCopyStream(&os);
		}
		m_WritePos += size;

		// Ŀ�� ��ġ ������ ó�� ����
		if (relocate == false)
		{
			return true;
		}

		// ��Ŷ�� �� ������ �����ؼ� Ŀ���� ������ �Űܾ� ��
		// Write�� �� �ִ� ũ�Ⱑ �ִ� ũ�� ���� ������ Ŀ�� ��ġ �ʱ�ȭ
		auto writableSize = m_BufferSize - m_WritePos;
		if (writableSize < m_MaxPacketSize)
		{
			// ó������ �����͵��� ������ ���ϰ� �ִ� ��
			// ������ ���� ���� Write �۾��� ���ж�� �Ǵ�
			if (m_PrevReadPos == 0)
			{
				return false;
			}

			// m_ReadPos, m_PrevReadPos�� �׻� m_WrtiePos���� �ռ����� �ʰ�
			// �׻� Write �� Read, ReadComplete ������ ȣ��� ����

			// Ŀ���� �� ������ �ű��.
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

		// Write�� ����������, ���� �� ���� ���� ũ�Ⱑ ��Ŷ �ִ� ũ�⺸�� ������ Ŀ�� ��ġ �ʱ�ȭ
		auto readableSize = m_BufferSize - m_ReadPos;
		if (relocate && readableSize < m_MaxPacketSize)
		{
			m_ReadPos = 0;
		}

		return readPos;
	}

	// ���� �����͸� ����ϰ� �ش� ũ�⸦ �����ϱ� ���� Ŀ�� ��ġ ������
	void ReadComplete(int length)
	{
		m_PrevReadPos += length;

		// ���������� ���� ���� ũ�� �˻� �� �ʱ�ȭ
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

	// �� ���� ���� �� �ִ� �������� �ִ� ũ�� = ��Ŷ�� �ִ� ũ��
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