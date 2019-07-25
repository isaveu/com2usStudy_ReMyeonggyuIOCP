#pragma once

#include "..//lsbIOCP/IServer.h"
#include "..//lsbIOCP/AsyncIOServer.h"

#include "ConnectedUserManager.h"
#include "RoomManager.h"
#include "UserManager.h"
#include "Packet.h"
#include "ErrorCode.h"

using ERROR_CODE = lsbLogic::ERROR_CODE;

namespace lsbLogic
{
	class PacketProcess
	{
		typedef ERROR_CODE(PacketProcess::* PacketFunc)(PacketInfo);
		PacketFunc PacketFuncArray[static_cast<int>(PACKET_ID::MAX)];

	public:
		void Init(
			LogicMain* const m_pLogicMain
			, UserManager* const pUserMngr
			, RoomManager* const pConfig
			, ServerConfig serverConfig
			, ConnectedUserManager* pConnUsrMngr
			, Log* const pLogger);

		void Process(PacketInfo packetInfo);
		void StateCheck();

	private:
		Log* m_Log;
		LogicMain* m_pLogicMain;
		UserManager* m_pUserMngr;
		RoomManager* m_pRoomMngr;
		ConnectedUserManager* m_pConnectedUserManager;
		ServerConfig m_ServerConfig;

	private:
		ERROR_CODE NtfSysConnctSession(PacketInfo packetInfo);

		ERROR_CODE NtfSysCloseSession(PacketInfo packetInfo);

		ERROR_CODE Login(PacketInfo packetInfo);

		ERROR_CODE Logout(PacketInfo packetInfo);

		ERROR_CODE RoomEnter(PacketInfo packetInfo);

		ERROR_CODE RoomLeave(PacketInfo packetInfo);

		ERROR_CODE RoomChat(PacketInfo packetInfo);

		ERROR_CODE DevEcho(PacketInfo packetInfo);
	};
}