#include "Worker.h"

#include "AsyncIOServer.h"

Worker::Worker(
	IServerReceiver* const pReceiver,
	const HANDLE iocpHandle,
	SessionManager* const pSessionManager,
	AsyncIOServer* const pServer,
	Log* const pLog)
	: m_pReceiver(pReceiver)
	, m_IOCPHandle(iocpHandle)
	, m_pSessionManager(pSessionManager)
	, m_pServer(pServer)
	, m_Log(pLog) {}

// Handle all of completed IOCP queue jobs from GQCSEX
void Worker::HandleCompletion()
{
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
			INFINITE,
			alertable);

		// Process completed job
		for (ULONG i = 0; i < removedNumber; i++)
		{
			auto nBytes = completionPortEntries[i].dwNumberOfBytesTransferred;
			auto lpOverlapped = completionPortEntries[i].lpOverlapped;
			auto sessionId = static_cast<INT>(completionPortEntries[i].lpCompletionKey);

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
void Worker::DispatchError(DWORD error, LPOVERLAPPED lpOverlapped, INT id)
{
	if (error == WAIT_TIMEOUT) return;

	m_Log->Write(LV::ERR, "#%u Dispatch Error", error);

	auto overlappedEx = reinterpret_cast<OVERLAPPED_EX*>(lpOverlapped);
	if (overlappedEx == nullptr)
	{
		m_Log->Write(LV::ERR, "#%u overlapped is null", error);
		return;
	}

	// If connectEx job, notify to server that connect job is fail
	if (overlappedEx->type == OP_TYPE::CONN)
	{
		// ref
		// https://siminq.tistory.com/entry/%EC%B0%B8%EC%A1%B0%EC%9E%90%EB%A5%BC-%EB%A6%AC%ED%84%B4%ED%95%98%EB%8A%94-%ED%95%A8%EC%88%98-%EC%9D%B4%ED%95%B4-%EC%89%BD%EA%B2%8C-%ED%95%98%EA%B8%B0
		// atmoic은 ref 사용 불허
		auto reqId = overlappedEx->requesterId;
		auto sessionDesc = m_pSessionManager->GetSessionDescRef(id);
		m_pReceiver->NotifyServerConnectingResult(sessionDesc, reqId, error);
		m_Log->Write(LV::ERR, "Can not connect to server");
	}

	// This connection has problem, so disconnect
	m_pServer->UnlinkSocketToSession(id, error);
}

// Process completed IO job
void Worker::DispatchCompleteion(DWORD transferredBytesNumber, LPOVERLAPPED lpOverlapped, INT sessionId)
{
	auto overlappedEx = reinterpret_cast<OVERLAPPED_EX*>(lpOverlapped);
	auto session = m_pSessionManager->GetSessionPtr(sessionId);

	// already socket disconnected
	if (session->IsOpened() == false)
	{
		m_Log->Write(LV::ERR, "Already disconnected connection");
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
			auto error = m_pSessionManager->PostRecv(session);
			if (error != FALSE)
			{
				m_Log->Write(LV::ERR, "#%u PostRecv failed", error);
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
			overlappedEx->bufferMngr.IncreseWrtiePos(transferredBytesNumber);
			overlappedEx->bufferMngr.PreventBufferOverflow();

			auto pData = overlappedEx->bufferMngr.ReadCurret();

			// If got messages, notify to server
			auto res = m_pReceiver->NotifyMessage(sessionDesc, transferredBytesNumber, pData);

			if (res)
			{
				overlappedEx->bufferMngr.IncreseReadPos(transferredBytesNumber);
			}
			// And post WSARecv again
			m_pSessionManager->PostRecv(session);
		}
		break;

		// Send message job
	case OP_TYPE::SEND:
		overlappedEx->bufferMngr.IncreseReadPos(transferredBytesNumber);
		break;

	default:
		m_Log->Write(LV::ERR, "Overlapped has no type");
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
	m_Log->Write(LV::INFO, "[%d] thread stoped handling completion job", ::GetCurrentThreadId());
}