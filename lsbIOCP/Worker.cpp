#include "Worker.h"

#include "AsyncIOServer.h"

Worker::Worker(IServerReceiver* pReceiver, HANDLE iocpHandle, SessionManager* pSessionManager, AsyncIOServer* pServer)
	: m_pReceiver(pReceiver)
	, m_IOCPHandle(iocpHandle)
	, m_pSessionManager(pSessionManager)
	, m_pServer(pServer)
	, m_Log(Log::GetInstance()) {}

// Handle all of completed IOCP queue jobs from GQCSEX
void Worker::HandleCompletion()
{
	// To consider thead stop, set wait time not INFINITY
	constexpr DWORD		waitTime = 500;
	constexpr ULONG		maxRemoveCount = 2;
	constexpr BOOL		alertable = FALSE;
	ULONG	removedNumber;
	OVERLAPPED_ENTRY completionPortEntries[maxRemoveCount];

	while (this->IsStart())
	{
		// Get completed IO jobs
		auto res = ::GetQueuedCompletionStatusEx(
			m_IOCPHandle,
			completionPortEntries,
			maxRemoveCount,
			&removedNumber,
			waitTime,
			alertable);

		// Process completed job
		for (ULONG i = 0; i < removedNumber; i++)
		{
			auto nBytes = completionPortEntries[i].dwNumberOfBytesTransferred;
			auto lpOverlapped = completionPortEntries[i].lpOverlapped;
			auto sessionId = completionPortEntries[i].lpCompletionKey;

			if (res == FALSE)
			{
				DispatchError(::GetLastError(), nBytes, lpOverlapped, sessionId);
			}
			else
			{
				DispatchCompleteion(nBytes, lpOverlapped, sessionId);
			}
		}
	}
}

// Process IO job having error
void Worker::DispatchError(DWORD error, DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, ULONG_PTR id)
{
	if (error == WAIT_TIMEOUT) return;

	m_Log->Write(utils::Format("[Error %u] Dispatch Error", error), LOG_LEVEL::DEBUG);

	auto overlappedEx = reinterpret_cast<LPOVERLAPPED_EX>(lpOverlapped);
	if (overlappedEx == nullptr)
	{
		m_Log->Write(utils::Format("[Error %u] overlapped is null", error), LOG_LEVEL::ERR);
		return;
	}

	// If connectEx job, notify to server that connect job is fail
	if (overlappedEx->type == OP_TYPE::CONN)
	{
		// ref
		// https://siminq.tistory.com/entry/%EC%B0%B8%EC%A1%B0%EC%9E%90%EB%A5%BC-%EB%A6%AC%ED%84%B4%ED%95%98%EB%8A%94-%ED%95%A8%EC%88%98-%EC%9D%B4%ED%95%B4-%EC%89%BD%EA%B2%8C-%ED%95%98%EA%B8%B0
		// atmoic�� ref ��� ����
		auto reqId = overlappedEx->requesterId;
		auto sessionDesc = m_pSessionManager->GetSessionDescRef(id);
		m_pReceiver->NotifyServerConnectingResult(sessionDesc, reqId, error);
		m_Log->Write("Can not connect to server", LOG_LEVEL::ERR);
	}

	// This connection has problem, so disconnect
	m_pServer->UnlinkSocketToSession(id, error);
}

// Process completed IO job
void Worker::DispatchCompleteion(DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, ULONG_PTR sessionId)
{
	auto overlappedEx = reinterpret_cast<LPOVERLAPPED_EX>(lpOverlapped);
	auto session = m_pSessionManager->GetSessionPtr(sessionId);

	// already socket disconnected
	if (session->IsOpened() == false)
	{
		m_Log->Write("Already disconnected connection", LOG_LEVEL::DEBUG);
		return;
	}

	auto sessionDesc = session->GetSessionDescRef();

	switch (overlappedEx->type)
	{
		// Job for ConnectEx
		// Notify to server the result
	case OP_TYPE::CONN:
		// Sokcet connected by ConnectEx() can receive packet after calling setsockopt()
		// So, session is opend after setsockopt()
		if (setsockopt(session->GetSocket(), SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0) != FALSE)
		{ 
			auto lastError = ::GetLastError();
			m_pReceiver->NotifyServerConnectingResult(sessionDesc, overlappedEx->requesterId, lastError);
			m_pServer->UnlinkSocketToSession(sessionId, lastError);
		}
		else
		{
			// If job goes smoothly, post WSARecv
			m_pReceiver->NotifyServerConnectingResult(sessionDesc, overlappedEx->requesterId, NULL);
			auto error = m_pServer->PostRecv(session);
			if (error != FALSE)
			{
				m_Log->Write(utils::Format("[Error %u] PostRecv failed", error), LOG_LEVEL::ERR);
				m_pServer->UnlinkSocketToSession(sessionId, error);
				return;
			}
			session->Open();
		}
		break;

		// Receive job
	case OP_TYPE::RECV:
		if (transferredBytesNumber == 0)
		{
			// If get zero bytes, disconnect
			m_pServer->UnlinkSocketToSession(sessionId, NULL);
		}
		else
		{
			// If got messages, notify to server
			m_pReceiver->NotifyMessage(sessionDesc, transferredBytesNumber, overlappedEx->wsabuf.buf);

			// And post WSARecv again
			m_pServer->PostRecv(session);
		}
		break;

		// Send message job
	case OP_TYPE::SEND:
		break;

	default:
		m_Log->Write("Overlapped has no type", LOG_LEVEL::ERR);
		break;
	}

	// atmoic �ϰ� �� �����͸� �����Ϸ���,
	// https://stackoverflow.com/questions/38984153/how-can-i-implement-aba-counter-with-c11-cas

	// ���� ���� ���¿��� ���� �ڵ�� ���� ��
	// �ٸ� �����忡�� ������ �ݾƹ����ٸ�??

	// CONN�� ��� ���ʿ� ���� ���̶� �ٸ� recv, send�� ���� ��. (������ �ϳ��� �۾� ����)

	// Recv�� ���
	// WSABUF�� ���ο� ���� ���� ������ ���� �ʱ� ������ �������

	// Send�� ���
	// WSABUF�� �������� ���� ����� -> close �ƴµ� ����ȵ�
	// ��¥ �쿬�� ������ ���� �ó����� �߻� ����
	// 1. [thread 1] close check - ���� �� ����
	// 2. [thread 2] socket close
	// 3. [thread 1] WSABUF�� ������ ����
	// 4. [thread 2] �ݾҴ� �ٷ� �� socket���� �ٸ� Ŭ���̾�Ʈ�� �����
	// 5. [thread 1] WSASend ȣ��
	// 6. ���� ����� Ŭ���̾�Ʈ�� �����Ͱ� �۽ŵ�
}

void Worker::Run()
{
	Worker::HandleCompletion();
	m_Log->Write(utils::Format("[%d] thread stoped handling completion job", ::GetCurrentThreadId()));
}