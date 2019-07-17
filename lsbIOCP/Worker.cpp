#include "Worker.h"

Worker::Worker(IServerReceiver* pReceiver, HANDLE iocpHandle, SessionManager* pSessionManager)
	: m_pReceiver(pReceiver)
	, m_IOCPHandle(iocpHandle)
	, m_pSessionManager(pSessionManager)
	, m_Log(Log::GetInstance()) {}

void Worker::HandleCompletion()
{
	constexpr DWORD		waitTime = 500;
	constexpr ULONG		maxRemoveCount = 2;
	constexpr BOOL		alertable = FALSE;
	ULONG	removedNumber;
	OVERLAPPED_ENTRY completionPortEntries[maxRemoveCount];

	while (this->IsStart())
	{
		auto res = ::GetQueuedCompletionStatusEx(
			m_IOCPHandle,
			completionPortEntries,
			maxRemoveCount,
			&removedNumber,
			waitTime,
			alertable);

		for (ULONG i = 0; i < removedNumber; i++)
		{
			auto nBytes = completionPortEntries[i].dwNumberOfBytesTransferred;
			auto lpOverlapped = completionPortEntries[i].lpOverlapped;
			auto sessionId = completionPortEntries[i].lpCompletionKey;

			if (res == FALSE) dispatchError(::GetLastError(), nBytes, lpOverlapped, sessionId);
			else dispatchCompleteion(nBytes, lpOverlapped, sessionId);
		}
	}
}

void Worker::dispatchError(DWORD error, DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, ULONG_PTR id)
{
	if (error == WAIT_TIMEOUT) return;

	auto overlappedEx = reinterpret_cast<LPOVERLAPPED_EX>(lpOverlapped);
	if (overlappedEx == nullptr)
	{
		// TODO: logging
		return;
	}

	if (overlappedEx->type == OP_TYPE::CONN)
	{
		// TODO: others
		// m_pSessionManager[id].
	}
}

void Worker::dispatchCompleteion(DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, ULONG_PTR id)
{

}

void Worker::Run()
{
	Worker::HandleCompletion();
	m_Log->Write(utils::Format("[%d] thread stoped handling completion job", ::GetCurrentThreadId()));
}