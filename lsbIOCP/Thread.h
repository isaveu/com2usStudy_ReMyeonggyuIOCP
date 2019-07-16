#pragma once
#include <windows.h>

#include <thread>

class Thread
{
public:
	Thread() = default;
	Thread(std::thread* t);
	virtual ~Thread();

	virtual	bool	IsStart();
	virtual void	Run() = 0;
	virtual	DWORD	Start();
	virtual	void	Stop();
	virtual	void	Join();
	virtual	void	Sleep(long ms);
	virtual	bool	WaitFor();

protected:
	std::thread*	m_Runner = nullptr;
	HANDLE			m_Handle = INVALID_HANDLE_VALUE;
	volatile bool	m_IsStarted = false;

private:
	void ValidateHandle();
};

