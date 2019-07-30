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
        public const int MAX_USER_ID_BYTE_LENGTH = 16;
        public const int MAX_USER_PW_BYTE_LENGTH = 16;
        public const int MAX_ROOM_TITLE_LENGTH = 16;
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

        PACKET_ID_ROOM_USER_LIST_NTF = 34,

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

        ERROR_CODE_ID_DUPLICATION = 211,
        ERROR_CODE_MAX_USER_COUNT = 212,
        ERROR_CODE_INVALID_SESSION_INDEX = 213,
        ERROR_CODE_NOT_CONFIRM_USER = 214,
        ERROR_CODE_REMOVE_INVALID_SESSION = 221,

        ERROR_CODE_ENTER_INVALID_DOMAIN = 271,
        ERROR_CODE_ENTER_INVALID_LOBBY_INDEX = 272,
        ERROR_CODE_ENTER_INVALID_ROOM_INDEX = 273,
        ERROR_CODE_ENTER_CREATE_FAIL = 274,
        ERROR_CODE_ENTER_NOT_CREATED = 275,
        ERROR_CODE_ENTER_MEMBER_FULL = 276,
        ERROR_CODE_ENTER_EMPTY_ROOM = 277,

        ERROR_CODE_LEAVE_INVALID_DOMAIN = 286,
        ERROR_CODE_LEAVE_INVALID_LOBBY_INDEX = 287,
        ERROR_CODE_LEAVE_INVALID_ROOM_INDEX = 288,
        ERROR_CODE_LEAVE_NOT_CREATED = 289,
        ERROR_CODE_LEAVE_NOT_MEMBER = 290,

        ERROR_CODE_CHAT_INVALID_DOMAIN = 296,
        ERROR_CODE_CHAT_INVALID_LOBBY_INDEX = 297,
        ERROR_CODE_CHAT_INVALID_ROOM_INDEX = 298,
    }

    struct PacketData
    {
        public Int16 DataSize;  // packet body size
        public Int16 PacketID;
        public SByte Type;
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

    // Login
    public class LoginReqPacket
    {
        byte[] UserID = new byte[PacketDef.MAX_USER_ID_BYTE_LENGTH];
        byte[] UserPW = new byte[PacketDef.MAX_USER_PW_BYTE_LENGTH];

        public void SetValue(string userID, string userPW)
        {
            Encoding.UTF8.GetBytes(userID).CopyTo(UserID, 0);
            Encoding.UTF8.GetBytes(userPW).CopyTo(UserPW, 0);
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(UserID);
            dataSource.AddRange(UserPW);
            return dataSource.ToArray();
        }
    }

    public class LoginResPacket
    {
        public Int16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    // Logout
    public class LogoutReqPacket
    {
    }

    public class LogoutResPacket
    {
        public Int16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class RoomEnterReqPacket
    {
        bool IsCreate;
        Int16 RoomNumber;
        byte[] Title = new byte[PacketDef.MAX_ROOM_TITLE_LENGTH];
        public void SetValue(bool create, Int16 roomNumber, string title)
        {
            IsCreate = create;
            RoomNumber = roomNumber;
            Encoding.UTF8.GetBytes(title).CopyTo(Title, 0);
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes(IsCreate));
            dataSource.AddRange(BitConverter.GetBytes(RoomNumber));
            dataSource.AddRange(Title);
            return dataSource.ToArray();
        }
    }

    public class RoomEnterResPacket
    {
        public Int16 Result;
        public Int16 RoomIndex;
        public string RoomTitle;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            RoomIndex = BitConverter.ToInt16(bodyData, 2);
            RoomTitle = Encoding.UTF8.GetString(bodyData, 4, PacketDef.MAX_ROOM_TITLE_LENGTH);

            return true;
        }
    }

    public class RoomUserListNtfPacket
    {
        public Int16 Result;
        public int UserCount = 0;
        public List<Int16> UserUniqueIdList = new List<Int16>();
        public List<string> UserIDList = new List<string>();

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);

            var readPos = 2;
            var userCount = BitConverter.ToInt16(bodyData, readPos);
            readPos += 2;

            for (int i = 0; i < userCount; ++i)
            {
                var uniqeudId = BitConverter.ToInt16(bodyData, readPos);
                readPos += 2;

                var id = Encoding.UTF8.GetString(bodyData, readPos, PacketDef.MAX_USER_ID_BYTE_LENGTH).Split('\0')[0];
                readPos += PacketDef.MAX_USER_ID_BYTE_LENGTH + 1;

                UserUniqueIdList.Add(uniqeudId);
                UserIDList.Add(id);
            }

            UserCount = userCount;
            return true;
        }
    }

    public class RoomNewUserNtfPacket
    {
        public Int16 UserUniqueId;
        public string UserID;

        public bool FromBytes(byte[] bodyData)
        {
            UserUniqueId = BitConverter.ToInt16(bodyData, 0);
            UserID = Encoding.UTF8.GetString(bodyData, 2, PacketDef.MAX_USER_ID_BYTE_LENGTH).Split('\0')[0];
            return true;
        }
    }

    public class RoomLeaveResPacket
    {
        public Int16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class RoomLeaveUserNtfPacket
    {
        public Int16 UserUniqueId;

        public bool FromBytes(byte[] bodyData)
        {
            UserUniqueId = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class RoomChatReqPacket
    {
        Int16 MsgLen;
        byte[] Msg;//= new byte[PacketDef.MAX_USER_ID_BYTE_LENGTH];

        public void SetValue(string message)
        {
            Msg = Encoding.UTF8.GetBytes(message);
            MsgLen = (Int16)Msg.Length;
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes(MsgLen));
            dataSource.AddRange(Msg);
            return dataSource.ToArray();
        }
    }

    public class RoomChatResPacket
    {
        public Int16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class RoomChatNtfPacket
    {
        public Int16 UserUniqueId;
        public string Message;

        public bool FromBytes(byte[] bodyData)
        {
            UserUniqueId = BitConverter.ToInt16(bodyData, 0);
            var msgLen = BitConverter.ToInt16(bodyData, 2);
            byte[] messageTemp = new byte[msgLen];
            Buffer.BlockCopy(bodyData, 2 + 2, messageTemp, 0, msgLen);
            Message = Encoding.UTF8.GetString(messageTemp);
            return true;
        }
    }
}
