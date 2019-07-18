#pragma once

#include <winsock2.h>

#include <atomic>
#include <mutex>
#include <string>

enum OP_TYPE { RECV, SEND, CONN };

// Overlapped extended struct
typedef struct _OVERLAPPED_EX
{
public:
	_OVERLAPPED_EX() : maxLength(0), type(OP_TYPE::RECV) { Init(); wsabuf.buf = nullptr; }
	_OVERLAPPED_EX(size_t maxLength) : type(OP_TYPE::RECV), maxLength(maxLength)
	{
		Init();
		if (0 < maxLength) wsabuf.buf = new char[maxLength];
	}

	void Init()
	{
		ZeroMemory(&overlapped, sizeof(overlapped));
		wsabuf.len = 0;
		requesterId = 0;
	}

public:
	OVERLAPPED		overlapped;
	WSABUF			wsabuf;
	OP_TYPE			type;
	size_t			maxLength;			// Socket buffer max length // TODO: 없어도 괜찮지 않나?
	size_t			requesterId;		// [요청자가 건네는 구분값] for connectSocket()
} OVERLAPPED_EX, *LPOVERLAPPED_EX;

/***********************************************************************************************************/

class IServerController;
// Session descriptor (used in receiver)
// Users manipluate only this, when they need session information
typedef struct _SESSIONDESC
{
public:
	_SESSIONDESC() : id(-1), pController(nullptr) { ZeroMemory(&tick, sizeof(tick)); };
	_SESSIONDESC(IServerController* pCrtl) : id(-1), pController(pCrtl) { ZeroMemory(&tick, sizeof(tick)); }
	bool CheckTick(_SESSIONDESC& desc) const { return this->tick.QuadPart == desc.tick.QuadPart; }

public:
	short			id;
	std::string		ip;
	LARGE_INTEGER	tick;
	IServerController* pController;
} SESSIONDESC, *LPSESSIONDESC;

/***********************************************************************************************************/

// Session struct including Session descriptor
typedef struct _SESSION
{
public:
	_SESSION() = default;
	_SESSION(size_t ioBufMaxSize, IServerController* pController)
	{ 
		m_SessionDesc.pController = pController;
		m_pOverlappedRecv = new OVERLAPPED_EX(ioBufMaxSize);
		m_pOverlappedSend = new OVERLAPPED_EX(ioBufMaxSize);
		m_pOverlappedConn = new OVERLAPPED_EX(0);
		m_RefCount.store(0);
		m_IsOpened.store(false);
	}

	SOCKET GetSocket() const { return m_SocketId; }
	void SetController(IServerController* pController) { m_SessionDesc.pController = pController; }
	void SetSessionId(size_t id) { m_SessionDesc.id = static_cast<short>(id); }
	void InitOverlapped(OP_TYPE type) 
	{ 
		switch (type)
		{
		default:
		case OP_TYPE::CONN: m_pOverlappedConn->Init(); break;
		case OP_TYPE::RECV: m_pOverlappedRecv->Init(); break;
		case OP_TYPE::SEND: m_pOverlappedSend->Init(); break;
		}
	}

	LPOVERLAPPED_EX GetOverlapped(OP_TYPE type)
	{
		switch (type)
		{
		default:
		case OP_TYPE::CONN: return m_pOverlappedConn;
		case OP_TYPE::RECV: return m_pOverlappedRecv;
		case OP_TYPE::SEND: return m_pOverlappedSend;
		}
	}

	SESSIONDESC& GetSessionDescRef() { return m_SessionDesc; }

	void OpenSocket(SOCKET socket) { m_SocketId = socket; m_IsOpened.store(true); }
	void CloseSocket()
	{
		if (m_SocketId == INVALID_SOCKET) return;
		::closesocket(m_SocketId);
		m_IsOpened.store(false);
		m_SocketId = INVALID_SOCKET;
	}

	void EnterIO() { m_RefCount++; }
	void ExitIO() { m_RefCount--; }

	bool IsOpened() { return m_IsOpened.load(); }
	bool IsPossibleClose()
	{ 
		bool open = true; 
		return m_IsOpened.compare_exchange_strong(open, false);
	}

private:
	SESSIONDESC			m_SessionDesc;

	SOCKET				m_SocketId = 0;

	// atomic is not copyable or movable
	// atomic(const atomic&) = delete;
	// atomic& operator=(const atomic&) = delete;
	std::atomic_int		m_RefCount;	// ref count for pending IO to check before return unused session to pool
	std::atomic_bool	m_IsOpened;

	std::mutex			m_SendLock;

	LPOVERLAPPED_EX		m_pOverlappedSend;
	LPOVERLAPPED_EX		m_pOverlappedRecv;
	LPOVERLAPPED_EX		m_pOverlappedConn;
} SESSION, *LPSESSION;