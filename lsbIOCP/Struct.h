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
	_OVERLAPPED_EX() : maxLength(0), type(OP_TYPE::RECV) { Init(); }
	_OVERLAPPED_EX(size_t maxLength) : type(OP_TYPE::RECV), maxLength(maxLength)
	{
		Init();
		if (0 < maxLength)
			wsabuf.buf = new char[maxLength];
	}

	void Init()
	{
		ZeroMemory(&overlapped, sizeof(overlapped));
		wsabuf.len = 0;
		wsabuf.buf = nullptr;
		requesterID = 0;
	}

public:
	OVERLAPPED		overlapped;
	WSABUF			wsabuf;
	OP_TYPE			type;
	size_t			maxLength;			// Socket buffer max length
	size_t			requesterID;		// [요청자가 건네는 구분값] for connectSocket()
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
	int				id;
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
		sessionDesc.pController = pController;
		m_pOverlappedRecv = new OVERLAPPED_EX(ioBufMaxSize);
		m_pOverlappedSend = new OVERLAPPED_EX(ioBufMaxSize);
		m_pOverlappedConn = new OVERLAPPED_EX(0);
		m_RefCount.store(0);
		m_IsOpened.store(false);
	}

	SOCKET GetSocket() const { return m_SocketId; }
	void SetSocket(SOCKET socket) { m_SocketId = socket; }
	void SetController(IServerController* pController) { sessionDesc.pController = pController; }
	void SetSessionId(size_t id) { sessionDesc.id = static_cast<int>(id); }

	void enterIO() { m_RefCount++; }
	void exitIO() { m_RefCount--; }

	bool IsOpened() { return m_IsOpened.load(); }
	bool IsPossibleClose()
	{ 
		bool open = true; 
		return m_IsOpened.compare_exchange_strong(open, false);
	}

private:
	SESSIONDESC			sessionDesc;

	SOCKET				m_SocketId = 0;
	std::atomic_int		m_RefCount;
	std::atomic_bool	m_IsOpened;

	std::mutex			m_SendLock;

	LPOVERLAPPED_EX		m_pOverlappedSend;
	LPOVERLAPPED_EX		m_pOverlappedRecv;
	LPOVERLAPPED_EX		m_pOverlappedConn;
} SESSION, *LPSESSION;