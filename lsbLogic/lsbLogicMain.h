#pragma once

<<<<<<< HEAD:lsbLogic/LogicMain.h
#include "../lsbIOCP/IServer.h"
#include "Packet.h"
#include "UserManager.h"
#include "RoomManager.h"
#include "PacketProcess.h"

namespace lsbLogic
{
	struct LogicConfig
	{
		short MaxRoomNumber;
		short MaxRoomUserNumber;
		bool IsLoginCheck;
	};

	class LogicMain : public IServerReceiver
	{
	public:
		void Start();

		// Manager pointer
		void Init(IServerController* pController, LogicConfig logicConfig, ServerConfig serverConfig);

		void SendMsg(const int sessionId, const short packetId, const short length, char* data);
		void ForceClose(const int sessionId);
		void Connect(const int reqId, const char* ip, unsigned short port);
=======
#include "..//lsbIOCP/IServer.h"

namespace lsbLogic
{
	class lsbLogicMain : public IServerReceiver
	{
		// Manager pointer
>>>>>>> parent of aa4de77... Add Logic:lsbLogic/lsbLogicMain.h


		/**************************************** IServerReceiver ****************************************/
		// Triggered when client socket is connected
		void NotifyClientConnected(SESSIONDESC& sessionDesc) const override;

		// Triggered when socket is disconnected
		void NotifyClientDisconnected(INT sessionId) const override;

		// Triggered when server get message from client
		void NotifyMessage(SESSIONDESC& sessionDesc, size_t bytesNumber, char* data) const override;

		// Triggered when the job of connecting to other server is completed
		void NotifyServerConnectingResult(SESSIONDESC& session, INT requrestId, DWORD error) const override;
<<<<<<< HEAD:lsbLogic/LogicMain.h

	private:
		AsyncIOServer* m_pNetwork;

		IServerController* m_pController;
		Log* m_Log;

		UserManager* m_pUserMngr;
		RoomManager* m_pRoomMngr;
		PacketProcess* m_pPacketProc;

		ServerConfig m_Config;
		LogicConfig m_LConfig;
=======
>>>>>>> parent of aa4de77... Add Logic:lsbLogic/lsbLogicMain.h
	};
}