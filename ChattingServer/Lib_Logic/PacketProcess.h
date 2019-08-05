#pragma once

#include "../../Lib_NetworkIOCP/INetwork.h"
#include "../../Lib_NetworkIOCP/AsyncIONetwork.h"

#include "ConnectedUserManager.h"
#include "RoomManager.h"
#include "UserManager.h"
#include "Common/Packet.h"
#include "Common/ErrorCode.h"

#pragma warning( push )
#pragma warning( disable : 4125 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 6387 )
#include "Protobuf/Packet.pb.h"
#include <google/protobuf/message.h>
#pragma warning ( pop )

using namespace google::protobuf;

namespace lsbLogic
{
	class PacketProcess
	{
		typedef ERROR_CODE(PacketProcess::* PacketFunc)(PacketInfo);
		PacketFunc PacketFuncArray[static_cast<int>(PACKET_ID::MAX)];

	public:
		void Init(LogicMain* const m_pLogicMain
			, UserManager* const pUserMngr
			, RoomManager* const pConfig
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

	private:
		bool ParseDataToProto(Message* pProto, char* pData, short size);

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