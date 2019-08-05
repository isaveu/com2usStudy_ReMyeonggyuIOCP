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
    }

    public enum PACKET_ID : ushort
    {
        PACKET_ID_ECHO_REQ = 241,
    }

    public enum ERROR_CODE : Int16
    {
        ERROR_NONE = 0,
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
}
