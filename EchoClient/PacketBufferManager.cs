using System;

namespace echoClient_csharp
{
    class PacketBufferManager
    {
        int ReadPos = 0;    // Read ffset of packet buffer
        int WritePos = 0;   // Write offset of packet buffer

        int HeaderSize = 0;     // Packet header size in protocol
        int MaxPacketSize = 0;  // Maximum packet size in protocol

        int BufferSize = 0;     // Total packet buffer size
        byte[] PacketBuffer;
        byte[] PacketBufferSwap;    // Temp buffer used to relocate packet buffer


        // Initialize packet buffer
        public bool Init(int size, int headerSize, int maxPacketSize)
        {
            // minimum size is greater than 2 * MaxPacketSize
            if (size < (maxPacketSize * 2) || size < 1 || headerSize < 1 || maxPacketSize < 1)
            {
                return false;
            }

            BufferSize = size;
            PacketBuffer = new byte[size];
            PacketBufferSwap = new byte[size];

            HeaderSize = headerSize;
            MaxPacketSize = maxPacketSize;

            return true;
        }

        // Write serialized byte data on packet buffer
        public bool Write(byte[] data, int pos, int size)
        {
            if (data == null || (data.Length < (pos + size)))
            {
                return false;
            }

            // Fail if avaliable packet buffer size is insufficient
            var remainBufferSize = BufferSize - WritePos;
            if (remainBufferSize < size)
            {
                return false;
            }

            // Write byte data to packet buffer & update write offset
            Buffer.BlockCopy(data, pos, PacketBuffer, WritePos, size);
            WritePos += size;

            // Check avaliable packet buffer size state
            if (NextFree() == false)
            {
                // If not, relocate packet buffer
                BufferRelocate();
            }
            return true;
        }

        // Read serialized byte data saved in packet buffer
        public ArraySegment<byte> Read()
        {
            // Check the readable data is bigger than packet header size
            var enableReadSize = WritePos - ReadPos;
            if (enableReadSize < HeaderSize)
            {
                return new ArraySegment<byte>();
            }

            // Check packet data size is valid
            // Note : in our protocol, the packet header size is '5'
            //      : first 2 bytes have total packet size,
            //      : second 2 bytes have packet ID
            //      : last 1 byte has no data
            // So, We can take packet data size by converting 2 bytes data in front of packet buffer 
            // That is why we use BitConverter.ToInt16 function in here.
            var packetDataSize = BitConverter.ToInt16(PacketBuffer, ReadPos);
            if (enableReadSize < packetDataSize)
            {
                // If readable packet size is less than packet data size,
                // there are more data to receive
                return new ArraySegment<byte>();
            }

            // read packet data & update read offeset of packet buffer
            var completePacketData = new ArraySegment<byte>(PacketBuffer, ReadPos, packetDataSize);
            ReadPos += packetDataSize;
            return completePacketData;
        }

        // Check packet buffer can save next packet even if having maximum packet size
        bool NextFree()
        {
            var enableWriteSize = BufferSize - WritePos;

            if (enableWriteSize < MaxPacketSize)
            {
                return false;
            }

            return true;
        }

        // Relocate packet buffer using already read portion.
        void BufferRelocate()
        {
            var enableReadSize = WritePos - ReadPos;

            // Push foward packet buffer on the part already read
            Buffer.BlockCopy(PacketBuffer, ReadPos, PacketBufferSwap, 0, enableReadSize);
            Buffer.BlockCopy(PacketBufferSwap, 0, PacketBuffer, 0, enableReadSize);

            // Update offsets
            ReadPos = 0;
            WritePos = enableReadSize;
        }
    }
}
