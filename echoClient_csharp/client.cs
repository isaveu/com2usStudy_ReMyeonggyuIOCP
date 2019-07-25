using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Windows.Forms;
using System.Windows.Threading;

namespace echoClient_csharp
{
    public partial class client : Form
    {
        ClientTcp Socket = new ClientTcp();

        bool IsNetworkThreadRunning = false;
        bool IsBackGroundProcessRunning = false;

        Thread NetworkReadThread = null;
        Thread NetworkSendThread = null;

        PacketBufferManager PacketBuffer = new PacketBufferManager();
        Queue<PacketData> RecvPacketQueue = new Queue<PacketData>();
        Queue<byte[]> SendPacketQueue = new Queue<byte[]>();

        DispatcherTimer dispatcherUITimer;

        public client()
        {
            InitializeComponent();
        }

        private void Client_Load(object sender, EventArgs e)
        {
            PacketBuffer.Init((8096 * 10), PacketDef.PACKET_HEADER_SIZE, 1024);
            Log.Init(LOG_LEVEL.INFO);

            // Run packet send, recv thread
            IsNetworkThreadRunning = true;
            NetworkReadThread = new Thread(NetworkReadProcess);
            NetworkReadThread.Start();

            NetworkSendThread = new Thread(NetworkSendProcess);
            NetworkSendThread.Start();

            // Run background process per 0.1s
            IsBackGroundProcessRunning = true;
            dispatcherUITimer = new DispatcherTimer();
            dispatcherUITimer.Tick += new EventHandler(BackGroundProcess);
            dispatcherUITimer.Interval = new TimeSpan(0, 0, 0, 0, 100);
            dispatcherUITimer.Start();

            btnDisconnect.Enabled = false;

            SetPacketHandler();
            Log.Write("Client run", LOG_LEVEL.INFO);
        }

        private void Client_FormClosed(object sender, FormClosedEventArgs e)
        {
            IsNetworkThreadRunning = false;
            IsBackGroundProcessRunning = false;

            Socket.Close();
        }

        private void BtnConnect_Click(object sender, EventArgs e)
        {
            string address = serverIP.Text;

            if (checkBoxLocalhost.Checked)
            {
                address = "127.0.0.1";
            }

            int port = Convert.ToInt32(serverPort.Text);

            if (Socket.Connect(address, port))
            {
                currentStateText.Text = string.Format("{0}. Connected", DateTime.Now);
                btnConnect.Enabled = false;
                btnDisconnect.Enabled = true;

                Log.Write($"connected to server {address}", LOG_LEVEL.INFO);
            }
            else
            {
                currentStateText.Text = string.Format("{0}. Fail to connect", DateTime.Now);
            }
        }

        private void BtnDisconnect_Click(object sender, EventArgs e)
        {
            SetDisconnectd();
            Socket.Close();
        }

        // Recv packet
        void NetworkReadProcess()
        {
            const Int16 PacketHeaderSize = PacketDef.PACKET_HEADER_SIZE;
            const Int16 PacketErrorCodeSize = PacketDef.PACKET_ERROR_CODE_SIZE;

            while (IsNetworkThreadRunning)
            {
                if (false == Socket.IsConnected())
                {
                    Thread.Sleep(1);
                    continue;
                }

                var recvData = Socket.Receive();
                if (recvData != null)
                {
                    // Save recv data to packet buffer
                    PacketBuffer.Write(recvData.Item2, 0, recvData.Item1);

                    while (true)
                    {
                        var data = PacketBuffer.Read();
                        if (data.Count < 1) // There is no full packet
                        {
                            break;
                        }
                        
                        var packet = new PacketData();
                        // Get packet body size (total byte size - header bytes)
                        packet.DataSize = (Int16)(data.Count - PacketHeaderSize);

                        // Get packet id (serialized next 2 bytes)
                        packet.PacketID = BitConverter.ToInt16(data.Array, data.Offset + 2);

                        // Get packet type (fifth byte)
                        packet.Type = (SByte)data.Array[(data.Offset + 4)];

                        // Get packet error code
                        packet.ErrorCde = BitConverter.ToInt16(data.Array, data.Offset + 5);

                        // Get packet body (the following)
                        packet.BodyData = new byte[packet.DataSize];
                        Buffer.BlockCopy(data.Array, (data.Offset + PacketHeaderSize + PacketErrorCodeSize), packet.BodyData, 0, packet.DataSize);

                        lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
                        {
                            // Save recv data packet in recv queue
                            RecvPacketQueue.Enqueue(packet);
                        }
                    }
                    // Log.Write($"recv Data: {recvData.Item2}", LOG_LEVEL.INFO);
                }
                else
                {
                    Socket.Close();
                    SetDisconnectd();
                    Log.Write("Disconnected", LOG_LEVEL.INFO);
                }
            }
        }

        // Send packet
        void NetworkSendProcess()
        {
            while (IsNetworkThreadRunning)
            {
                Thread.Sleep(1);

                if (Socket.IsConnected() == false)
                {
                    continue;
                }

                lock (((System.Collections.ICollection)SendPacketQueue).SyncRoot)
                {
                    // If there exists packet to send in sendQueue,
                    // pop one packet in queue and send to socket now
                    if (SendPacketQueue.Count > 0)
                    {
                        var packet = SendPacketQueue.Dequeue();
                        Socket.Send(packet);
                    }
                }
            }
        }

        // Log & check there exists recv packet
        void BackGroundProcess(object sender, EventArgs e)
        {
            ProcessLog();
            try
            {
                var packet = new PacketData();

                lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
                {
                    // Check there exists recv packet in recv queue
                    if (RecvPacketQueue.Count() > 0)
                    {
                        packet = RecvPacketQueue.Dequeue();
                    }
                }

                if (packet.PacketID != 0)
                {
                    PacketProcess(packet);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("ReadPacketQueueProcess. error:{0}", ex.Message));
            }
        }

        private void ProcessLog()
        {
            // log work limit
            int logWorkCount = 0;

            while (IsBackGroundProcessRunning)
            {
                System.Threading.Thread.Sleep(1);

                string msg;

                // Pop log message and show on the form
                if (Log.GetLog(out msg))
                {
                    ++logWorkCount;

                    if (logList.Items.Count > 512)
                    {
                        logList.Items.Clear();
                    }

                    logList.Items.Add(msg);
                    logList.SelectedIndex = logList.Items.Count - 1;
                }
                else
                {
                    break;
                }

                if (logWorkCount > 8)
                {
                    break;
                }
            }
        }

        public void SetDisconnectd()
        {
            if (btnConnect.Enabled == false)
            {
                btnConnect.Enabled = true;
                btnDisconnect.Enabled = false;
            }

            SendPacketQueue.Clear();

            currentStateText.Text = "Disconnected";
        }

        public void PostSendPacket(PACKET_ID packetID, byte[] bodyData)
        {
            if (false == Socket.IsConnected())
            {
                Log.Write("Not connected to server", LOG_LEVEL.ERROR);
                return;
            }

            Int16 bodyDataSize = bodyData == null ? (Int16)0 : (Int16)bodyData.Length;

            // Total packet size is "Header + packet body"
            var packetSize = bodyDataSize + PacketDef.PACKET_HEADER_SIZE;

            // Packet header of our protocol as follows
            // First 2 bytes : total packet size
            // Second 2 bytes : packet Id
            // last 1 byte : packet type
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes((Int16)packetSize));
            dataSource.AddRange(BitConverter.GetBytes((Int16)packetID));
            dataSource.AddRange(new byte[] { (byte)0 });

            if (bodyData != null)
            {
                dataSource.AddRange(bodyData);
            }

            SendPacketQueue.Enqueue(dataSource.ToArray());
        }

        private void BtnEchoSend_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(echoMsg.Text))
            {
                MessageBox.Show("Write text message to send");
                return;
            }

            var body = Encoding.UTF8.GetBytes(echoMsg.Text);

            PostSendPacket(PACKET_ID.PACKET_ID_ECHO_REQ, body);

            Log.Write($"Echo req:  {echoMsg.Text}, {body.Length}");
        }
    }
}
