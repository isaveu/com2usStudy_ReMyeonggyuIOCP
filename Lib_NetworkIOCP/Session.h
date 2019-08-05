#pragma once

#include "Define.h"
#include "PacketBufferManager.h"

// Session struct including Session descriptor
class SESSION
{
public:
	SESSION() = default;
	SESSION(PacketBufferConfig packetBufferConfig)
	{
		m_pOverlappedRecv = new OVERLAPPED_EX(packetBufferConfig, OP_TYPE::RECV);
		m_pOverlappedSend = new OVERLAPPED_EX(packetBufferConfig, OP_TYPE::SEND);
		m_pOverlappedConn = new OVERLAPPED_EX();
		m_RefCount.store(0);
		m_IsOpened.store(false);
	}
	~SESSION()
	{
		delete m_pOverlappedConn;
		delete m_pOverlappedRecv;
		delete m_pOverlappedSend;
	}

	SOCKET GetSocket() const
	{
		return m_SocketId;
	}

	void SetSessionId(int id)
	{
		m_SessionId = id;
	}

	void Clear()
	{
		m_SocketId = 0;
		m_RefCount.store(0);
		m_IsOpened.store(false);
		m_pOverlappedConn->Clear();
		m_pOverlappedRecv->Clear();
		m_pOverlappedSend->Clear();
	}

	int GetSessionId()
	{
		return m_SessionId;
	}

	OVERLAPPED_EX* GetOverlapped(OP_TYPE type)
	{
		switch (type)
		{
		default:
		case OP_TYPE::CONN: return m_pOverlappedConn;
		case OP_TYPE::RECV: return m_pOverlappedRecv;
		case OP_TYPE::SEND: return m_pOverlappedSend;
		}
	}

	void SetSocket(SOCKET socket)
	{
		m_SocketId = socket;
	}

	bool Open()
	{
		bool open = false;
		return m_IsOpened.compare_exchange_strong(open, true);
	}

	bool Close()
	{
		bool open = true;
		return m_IsOpened.compare_exchange_strong(open, false);
	}

	void EnterIO()
	{
		m_RefCount++;
	}

	void ExitIO()
	{
		m_RefCount--;
	}

	bool IsOpened()
	{
		return m_IsOpened.load();
	}

public:
	std::mutex			m_SendLock;

private:
	int		m_SessionId = -1;
	SOCKET	m_SocketId = 0;

	// atomic is not copyable or movable
	// atomic(const atomic&) = delete;
	// atomic& operator=(const atomic&) = delete;
	std::atomic_int		m_RefCount;	// ref count for pending IO to check before return unused session to pool
	std::atomic_bool	m_IsOpened;

	OVERLAPPED_EX* m_pOverlappedSend;
	OVERLAPPED_EX* m_pOverlappedRecv;
	OVERLAPPED_EX* m_pOverlappedConn;
};