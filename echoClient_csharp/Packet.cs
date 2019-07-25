using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace echoClient_csharp
{
    // Packet definition about our protocol
    class PacketDef
    {
        public const Int16 PACKET_HEADER_SIZE = 5;
        public const Int16 PACKET_ERROR_CODE_SIZE = 2;
        public const int MAX_USER_ID_BYTE_LENGTH = 16;
        public const int MAX_USER_PW_BYTE_LENGTH = 16;
    }

    public enum PACKET_ID : ushort
    {
        PACKET_ID_ECHO_REQ = 241,

        PACKET_ID_ECHO_RES = 242,

        PACKET_ID_PING_REQ = 201,   // Ping(Heart-beat)

        PACKET_ID_PING_RES = 202,   // Ping(Heart-beat)

        PACKET_ID_ERROR_NTF = 203,

        PACKET_ID_LOGIN_REQ = 21,
        PACKET_ID_LOGIN_RES = 22,

        PACKET_ID_LOGOUT_REQ = 26,
        PACKET_ID_LOGOUT_RES = 27,

        PACKET_ID_ROOM_ENTER_REQ = 31,
        PACKET_ID_ROOM_ENTER_RES = 32,
        PACKET_ID_ROOM_ENTER_NEW_USER_NTF = 33,

        PACKET_ID_ROOM_LEAVE_REQ = 36,
        PACKET_ID_ROOM_LEAVE_RES = 37,
        PACKET_ID_ROOM_LEAVE_USER_NTF = 38,

        PACKET_ID_ROOM_CHAT_REQ = 42,
        PACKET_ID_ROOM_CHAT_RES = 42,
        PACKET_ID_ROOM_CHAT_NTF = 43,

        ROOM_CHANGED_INFO_NTF = 46,
    }

    public enum ERROR_CODE : Int16
    {
        ERROR_NONE = 0,

        ERROR_CODE_USER_MGR_INVALID_USER_UNIQUEID = 112,

        ERROR_CODE_PUBLIC_CHANNEL_IN_USER = 114,

        ERROR_CODE_PUBLIC_CHANNEL_INVALIDE_NUMBER = 115,
    }

    struct PacketData
    {
        public Int16 DataSize;  // packet body size
        public Int16 PacketID;
        public SByte Type;
        public Int16 ErrorCde;
        public byte[] BodyData;
    }

    public class PacketDump
    {
        public static string Bytes(byte[] byteArr)
        {
            StringBuilder sb = new StringBuilder("[");
            for (int i = 0; i < byteArr.Length; ++i)
            {
                sb.Append(byteArr[i] + " ");
            }
            sb.Append("]");
            return sb.ToString();
        }
    }

    public class ErrorNtfPacket
    {
        public ERROR_CODE Error;

        public bool FromBytes(byte[] bodyData)
        {
            Error = (ERROR_CODE)BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }
}
