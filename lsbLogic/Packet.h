#pragma once

#include "..//lsbIOCP/IServer.h"

#include "PacketID.h"
#include "ErrorCode.h"

namespace lsbLogic
{
	struct PacketInfo
	{
		PacketInfo() = default;
		int SessionId = -1;
		short PacketId = -1;
		short PacketBodySize = 0;
		char* pData = 0;
	};

#pragma pack(push, 1)
	struct PacketHeader
	{
		short PacketSize;
		short Id;
		unsigned char Reserved;
	};
	const int PACKET_HEADER_SIZE = sizeof(PacketHeader);
	const int PACKET_MAX_SIZE = 300;

	struct PacketBase
	{
		void SetErrorCode(ERROR_CODE error)
		{
			ErrorCode = static_cast<short>(error);
		}
		short ErrorCode = static_cast<short>(ERROR_CODE::NONE);
	};

	// Packet struct for login req, res
	const int MAX_USER_ID_SIZE = 16;
	const int MAX_USER_PW_SIZE = 16;
	struct PacketLoginReq
	{
		char szID[MAX_USER_ID_SIZE + 1] = { 0, };
		char szPW[MAX_USER_PW_SIZE + 1] = { 0, };
	};

	struct PacketLoginRes : public PacketBase
	{
	};

	struct PacketLogoutReq
	{
	};

	struct PacketLogoutRes : public PacketBase
	{
	};

	// Packet struct for entering room
	const int MAX_ROOM_TITLE_SIZE = 16;
	struct PacketRoomEnterReq
	{
		bool IsCreate;
		short RoomIndex;
		wchar_t RoomTitle[MAX_ROOM_TITLE_SIZE + 1];
	};

	struct PacketRoomEnterRes : public PacketBase
	{
	};

	struct PacketRoomEnterNtf
	{
		char UserId[MAX_USER_ID_SIZE + 1] = { 0, };
	};

	// Packet struct for leaving room
	struct PacketRoomLeaveReq
	{
	};

	struct PacketRoomLeaveRes : public PacketBase
	{
	};

	struct PacketRoomLeaveNtf
	{
		char UserId[MAX_USER_ID_SIZE + 1] = { 0, };
	};

	// Packet struct for chatting in room
	const int MAX_ROOM_CHAT_MSG_SIZE = 256;
	struct PacketRoomChatReq
	{
		wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE + 1] = { 0, };
	};

	struct PacketRoomChatRes : public PacketBase
	{
	};

	struct PacketRoomChatNtf
	{
		char UserId[MAX_USER_ID_SIZE + 1] = { 0, };
		wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE + 1] = { 0, };
	};

	// Packet struct for echo
	const short DEV_ECHO_DATA_MAX_SIZE = 1024;
	struct PacketEchoReq
	{
		char Data[DEV_ECHO_DATA_MAX_SIZE];
	};

	struct PacketEchoRes : public PacketBase
	{
		char Data[DEV_ECHO_DATA_MAX_SIZE];
	};
#pragma pack(pop)
}