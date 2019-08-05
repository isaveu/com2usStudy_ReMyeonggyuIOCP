using System;
using System.Text;
using System.Collections.Generic;
using LsbProto;

namespace echoClient_csharp
{
    public partial class client
    {
        Dictionary<PACKET_ID, Action<byte[]>> PacketFuncDic = new Dictionary<PACKET_ID, Action<byte[]>>();

        void SetPacketHandler()
        {
            PacketFuncDic.Add(PACKET_ID.PACKET_ID_ECHO_REQ, PacketProcess_Echo);
            //PacketFuncDic.Add(PACKET_ID.PACKET_ID_ROOM_RELAY_NTF, PacketProcess_RoomRelayNotify);
        }

        void PacketProcess(PacketData packet)
        {
            var packetType = (PACKET_ID)packet.PacketID;
            // Log.Write("RawPacket: " + packet.PacketID.ToString() + ", " + PacketDump.Bytes(packet.BodyData));

            if (PacketFuncDic.ContainsKey(packetType))
            {
                PacketFuncDic[packetType](packet.BodyData);
            }
            else
            {
                Log.Write("Unknown Packet Id: " + packet.PacketID.ToString());
            }
        }

        void PacketProcess_Echo(byte[] bodyData)
        {
            /* previous packet format code
            string body = Encoding.UTF8.GetString(bodyData);
            */

            Echo resPkt = Echo.Parser.ParseFrom(bodyData);
            Log.Write($"Echo Res : {resPkt.Msg}");
        }
    }
}
