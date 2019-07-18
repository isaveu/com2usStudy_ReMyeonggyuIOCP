#include "Worker.h"

#include "AsyncIOServer.h"

Worker::Worker(IServerReceiver* pReceiver, HANDLE iocpHandle, SessionManager* pSessionManager, AsyncIOServer* pServer)
	: m_pReceiver(pReceiver)
	, m_IOCPHandle(iocpHandle)
	, m_pSessionManager(pSessionManager)
	, m_pServer(pServer)
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

void Worker::DispatchError(DWORD error, DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, ULONG_PTR id)
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
		// ref
		// https://siminq.tistory.com/entry/%EC%B0%B8%EC%A1%B0%EC%9E%90%EB%A5%BC-%EB%A6%AC%ED%84%B4%ED%95%98%EB%8A%94-%ED%95%A8%EC%88%98-%EC%9D%B4%ED%95%B4-%EC%89%BD%EA%B2%8C-%ED%95%98%EA%B8%B0
		// atmoic은 ref 사용 불허
		auto reqId = overlappedEx->requesterId;
		auto sessionDesc = m_pSessionManager->GetSessionDescRef(id);
		m_pReceiver->NotifyServerConnectingResult(sessionDesc, reqId, error);
	}

	m_pServer->ReleaseSession(id, error);
}

void Worker::DispatchCompleteion(DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, ULONG_PTR id)
{
	auto overlappedEx = reinterpret_cast<LPOVERLAPPED_EX>(lpOverlapped);
	auto session = m_pSessionManager->GetSessionPtr(id);

	// already socket disconnected
	if (session->IsOpened() == false) return;

	auto reqId = overlappedEx->requesterId;
	auto sessionDesc = session->GetSessionDescRef();

	switch (overlappedEx->type)
	{
	case OP_TYPE::CONN:
		if (setsockopt(session->GetSocket(), SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0) != NULL)
		{ 
			auto lastError = ::GetLastError();
			m_pReceiver->NotifyServerConnectingResult(sessionDesc, reqId, lastError);
			m_pServer->ReleaseSession(id, lastError);
		}
		else
		{
			m_pReceiver->NotifyServerConnectingResult(sessionDesc, reqId, NULL);
			m_pServer->PostRecv(session);
		}
		break;

	case OP_TYPE::RECV:
		if (transferredBytesNumber == 0)
		{
			m_pServer->ReleaseSession(id, 0);
		}
		else
		{
			m_pReceiver->NotifyMessage(sessionDesc, transferredBytesNumber, overlappedEx->wsabuf.buf);
			m_pServer->PostRecv(session);
		}
		break;

	case OP_TYPE::SEND:
		break;

	default:
		// TODO: logging critical error
		break;
	}

	// atmoic 하게 두 데이터를 수정하려면,
	// https://stackoverflow.com/questions/38984153/how-can-i-implement-aba-counter-with-c11-cas

	// 만약 열린 상태에서 다음 코드로 진입 후
	// 다른 스레드에서 소켓을 닫아버린다면??

	// CONN의 경우 애초에 연결 전이라서 다른 recv, send가 없을 것. (유일한 하나의 작업 보장)

	// Recv의 경우
	// WSABUF에 새로운 값을 쓰는 행위를 하지 않기 때문에 상관없음

	// Send의 경우
	// WSABUF에 보내려는 값을 써야함 -> close 됐는데 쓰면안됨
	// 진짜 우연히 다음과 같은 시나리오 발생 가능
	// 1. [thread 1] close check - 아직 안 닫힘
	// 2. [thread 2] socket close
	// 3. [thread 1] WSABUF에 데이터 갱신
	// 4. [thread 2] 닫았던 바로 그 socket으로 다른 클라이언트가 연결됨
	// 5. [thread 1] WSASend 호출
	// 6. 새로 연결된 클라이언트로 데이터가 송신됨
}

void Worker::Run()
{
	Worker::HandleCompletion();
	m_Log->Write(utils::Format("[%d] thread stoped handling completion job", ::GetCurrentThreadId()));
}