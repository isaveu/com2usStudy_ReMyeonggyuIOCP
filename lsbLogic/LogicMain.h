#pragma once

#include <concurrent_queue.h>

#include "../lsbIOCP/Struct.h"

#include "Packet.h"
#include "LogicConfig.h"

class Log;
class AsyncIOServer;

namespace lsbLogic
{
	class UserManager;
	class RoomManager;
	class ConnectedUserManager;
	class PacketProcess;

	class LogicMain : public IServerReceiver
	{
	public:
		ServerConfig m_Config;

	public:
		void Start();
		void Stop();
		void Run();

		// Manager pointer
		void Init(ServerConfig m_SConfig, LogicConfig m_LConfig);

		void SendMsg(const int sessionId, const short packetId, const short length, char* data);
		void ForceClose(const int sessionId);
		void ConnectServer(const int reqId, const char* ip, unsigned short port);

	private:
		/**************************************** IServerReceiver ****************************************/
		// Triggered when client socket is connected
		void NotifyClientConnected(SESSIONDESC& sessionDesc) override;

		// Triggered when socket is disconnected
		void NotifyClientDisconnected(INT sessionId) override;

		// Triggered when server get message from client
		bool NotifyMessage(SESSIONDESC& sessionDesc, size_t nBytes, char* pData) override;

		// Triggered when the job of connecting to other server is completed
		void NotifyServerConnectingResult(SESSIONDESC& session, INT requrestId, DWORD error) override;

	private:
		AsyncIOServer* m_pNetwork;
		UserManager* m_pUserMngr;
		RoomManager* m_pRoomMngr;
		ConnectedUserManager* m_pConnUserMngr;
		PacketProcess* m_pPktProc;

		ServerConfig m_SConfig;
		LogicConfig m_LConfig;

		bool m_IsRun = false;

		Concurrency::concurrent_queue<PacketInfo> m_PacketQueue;

		Log* m_Log;
	};
}