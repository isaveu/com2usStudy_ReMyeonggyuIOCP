#include "Worker.h"
#include "Session.h"
#include "AsyncIONetwork.h"

Worker::Worker(
	INetworkReceiver* const pReceiver,
	const HANDLE iocpHandle,
	SessionManager* const pSessionManager,
	AsyncIONetwork* const pServer,
	Log* const pLog)
	: m_pReceiver(pReceiver)
	, m_IOCPHandle(iocpHandle)
	, m_pSessionManager(pSessionManager)
	, m_pServer(pServer)
	, m_pLogger(pLog) {}

// Handle all of completed IOCP queue jobs from GQCSEX
void Worker::HandleCompletion()
{
	constexpr ULONG		maxRemoveCount = 2;
	constexpr BOOL		alertable = FALSE;
	constexpr DWORD		waitTime = INFINITE;
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
			auto sessionId = static_cast<int>(completionPortEntries[i].lpCompletionKey);

			if (res == FALSE)
			{
				DispatchError(::GetLastError(), lpOverlapped, sessionId);
			}
			else
			{
				DispatchCompleteion(nBytes, lpOverlapped, sessionId);
			}
		}
	}
}

// Process IO job having error
void Worker::DispatchError(DWORD error, LPOVERLAPPED lpOverlapped, int sessionId)
{
	if (error == WAIT_TIMEOUT)
	{
		return;
	}

	m_pLogger->Write(LV::ERR, "[ERROR #%u] Dispatch Error", error);

	auto overlappedEx = reinterpret_cast<OVERLAPPED_EX*>(lpOverlapped);
	if (overlappedEx == nullptr)
	{
		m_pLogger->Write(LV::ERR, "[ERROR #%u] overlapped is null", error);
		return;
	}

	// If connectEx job, notify to server that connect job is fail
	if (overlappedEx->type == OP_TYPE::CONN)
	{
		// ref
		// https://siminq.tistory.com/entry/%EC%B0%B8%EC%A1%B0%EC%9E%90%EB%A5%BC-%EB%A6%AC%ED%84%B4%ED%95%98%EB%8A%94-%ED%95%A8%EC%88%98-%EC%9D%B4%ED%95%B4-%EC%89%BD%EA%B2%8C-%ED%95%98%EA%B8%B0
		// atmoic�� ref ��� ����
		auto reqId = overlappedEx->requesterId;
		m_pReceiver->NotifyServerConnectingResult(sessionId, reqId, NET_ERROR_CODE::FAIL_CONNECTEX_PENDIG_JOB);
		m_pLogger->Write(LV::ERR, "[ERROR #%u] Can not connect to server", error);
	}

	// This connection has problem, so disconnect
	m_pServer->UnlinkSocketToSession(sessionId, NET_ERROR_CODE::DISPATCH_ERROR);
}

// Process completed IO job
void Worker::DispatchCompleteion(DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, int sessionId)
{
	auto overlappedEx = reinterpret_cast<OVERLAPPED_EX*>(lpOverlapped);
	auto session = m_pSessionManager->GetSessionPtr(sessionId);

	// already socket disconnected
	if (session->IsOpened() == false)
	{
		m_pLogger->Write(LV::ERR, "Already disconnected connection");
		return;
	}

	switch (overlappedEx->type)
	{
		// Job for ConnectEx
		// Notify to server the result
	case OP_TYPE::CONN:
		// Sokcet connected by ConnectEx() can receive packet after calling setsockopt()
		// So, session is opend after setsockopt()
		if (setsockopt(session->GetSocket(), SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0) != FALSE)
		{ 
			m_pLogger->Write(LV::ERR, "[ERROR #%u] Can not set socket option", ::GetLastError());
			m_pReceiver->NotifyServerConnectingResult(sessionId, overlappedEx->requesterId, NET_ERROR_CODE::FAIL_SETSOCKOPT);
			m_pServer->UnlinkSocketToSession(sessionId, NET_ERROR_CODE::FAIL_SETSOCKOPT);
		}
		else
		{
			// If job goes smoothly, post WSARecv
			m_pReceiver->NotifyServerConnectingResult(sessionId, overlappedEx->requesterId, NET_ERROR_CODE::NONE);
			auto error = m_pSessionManager->PostRecv(session);
			if (error != NET_ERROR_CODE::NONE)
			{
				m_pLogger->Write(LV::ERR, "[NET_ERROR #%d] PostRecv failed", error);
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
			m_pServer->UnlinkSocketToSession(sessionId, NET_ERROR_CODE::NONE);
		}
		else
		{
			auto isSuccess = overlappedEx->bufferMngr.IncreseWrtiePos(transferredBytesNumber);
			if (isSuccess == false)
			{
				// ������ Recv�� �����͸� ó������ ���ϰ� ���۰� �������ִ� ���... ������ ���� ������???
				m_pServer->UnlinkSocketToSession(sessionId, NET_ERROR_CODE::PACKET_BUFFER_FULL);
				return;
			}

			auto pData = overlappedEx->bufferMngr.Read(transferredBytesNumber);

			// If got messages, notify to server
			auto ret = m_pReceiver->NotifyMessage(sessionId, transferredBytesNumber, pData);

			// �������� �����͸� �о�ٸ� ���� ǥ��ó��
			// TODO: ������ �����͸� �а� �� �� ��Ŷ body �����͸� ��Ŷ ó�� ������� �Ѱ��� �� �ش� ��Ŷ �ٵ����Ͱ� ���� ���Ϸ� ���� ��
			// ���� ��Ŷ ó���� �ʰ�, Recv�ϴ� �����Ͱ� �������ٸ� ���� ó������ ���� ��Ŷ�� body �����Ͱ� ������� �� ����
			if (ret)
			{
				overlappedEx->bufferMngr.ReadComplete(transferredBytesNumber);
			}
			// And post WSARecv again
			m_pSessionManager->PostRecv(session);
		}
		break;

		// Send message job
	case OP_TYPE::SEND:
		// ���� �����͸� ���������� ���� ���۷� ��������
		// �����б� Ŀ���� �������Ͽ� ���� ����
		overlappedEx->bufferMngr.ReadComplete(transferredBytesNumber);
		break;

	default:
		m_pLogger->Write(LV::ERR, "Overlapped has no type");
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
	m_pLogger->Write(LV::INFO, "[Thread %d] Stoped handling completion job", ::GetCurrentThreadId());
}