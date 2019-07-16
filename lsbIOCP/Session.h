#pragma once

#include <windows.h>
#include <mutex>

#include "Struct.h"

class Session
{
private:
	std::mutex		m_SendLock;
	SOCKET			m_SocketId;
	int				m_RefCount;
	bool			m_Closed = true;
	OVERLAPPED_EX	m_Overlapped;
};
