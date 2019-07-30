#include "Thread.h"

void Thread::ValidateHandle()
{
	if (INVALID_HANDLE_VALUE != m_Handle) {
		CloseHandle(m_Handle);
	}
}

Thread::Thread(std::thread* t) { m_Runner = t; }

Thread::~Thread()
{
	if (m_Runner != nullptr)
	{
		Join();
	}
	Thread::ValidateHandle();
}

bool Thread::IsStart() 
{ 
	return m_IsStarted; 
}

void Thread::Sleep(long ms)
{ 
	::Sleep(ms); 
}

void Thread::Join()
{
	m_Runner->join();
}

DWORD Thread::Start()
{
	if (FALSE != m_IsStarted) {
		return ERROR_ALREADY_THREAD;
	}

	if (INVALID_HANDLE_VALUE != m_Handle) {
		CloseHandle(m_Handle);
	}

	m_IsStarted = true;
	m_Runner = new std::thread(&Thread::Run, this);
	m_Handle = m_Runner->native_handle();

	return 0;
}

void Thread::Stop()
{
	m_IsStarted = false;
}

bool Thread::WaitFor()
{
	DWORD r = ::WaitForSingleObject(m_Handle, INFINITE);

	if (r == WAIT_OBJECT_0) {
		return true;
	}

	return false;
}
