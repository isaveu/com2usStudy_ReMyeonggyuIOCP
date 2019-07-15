using System;
using System.Text;
using System.Collections.Generic;

namespace echoClient_csharp
{
    public partial class client
    {
        Dictionary<PACKET_ID, Action<byte[]>> PacketFuncDic = new Dictionary<PACKET_ID, Action<byte[]>>();

        void SetPacketHandler()
        {
            PacketFuncDic.Add(PACKET_ID.PACKET_ID_ECHO, PacketProcess_Echo);
            PacketFuncDic.Add(PACKET_ID.PACKET_ID_ERROR_NTF, PacketProcess_ErrorNotify);
        }

        void PacketProcess(PacketData packet)
        {
            var packetType = (PACKET_ID)packet.PacketID;
            Log.Write("RawPacket: " + packet.PacketID.ToString() + ", " + PacketDump.Bytes(packet.BodyData));

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
            string body = Encoding.UTF8.GetString(bodyData);
            Log.Write($"Echo : {body}");
        }

        void PacketProcess_ErrorNotify(byte[] bodyData)
        {
            var notifyPkt = new ErrorNtfPacket();
            notifyPkt.FromBytes(bodyData);

            Log.Write($"Error:  {notifyPkt.Error}");
        }
    }
}
