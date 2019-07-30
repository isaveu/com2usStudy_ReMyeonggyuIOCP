using System;
using System.Net.Sockets;
using System.Net;

namespace echoClient_csharp
{
    public class ClientTcp
    {
        public Socket Socket;
        public string Err;
        private int ReadBufferSize = 2048;

        public bool IsConnected()
        {
            return Socket != null && Socket.Connected;
        }
        
        public bool Connect(string serverIP, int serverPort)
        {
            try
            {
                IPAddress ipaddressServerIP = IPAddress.Parse(serverIP);

                Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                Socket.Connect(new IPEndPoint(ipaddressServerIP, serverPort));

                if (!IsConnected())
                {
                    Err = "Fail to connect to server";
                    return false;
                }

                return true;
            }
            catch (Exception e)
            {
                Err = e.Message;
                return false;
            }
        }

        // Receive data from socket
        // return <byte size of recv data, recv data bytes>
        public Tuple<int, byte[]> Receive()
        {
            try
            {
                byte[] readBuffer = new byte[ReadBufferSize];
                var recvBytes = Socket.Receive(readBuffer, 0, readBuffer.Length, SocketFlags.None);

                if (recvBytes == 0)
                {
                    return null;
                }

                return Tuple.Create(recvBytes, readBuffer);
            }
            catch (SocketException se)
            {
                Err = se.Message;
                return null;
            }
        }

        public void Send(byte[] data)
        {
            try
            {
                if (IsConnected())
                {
                    Socket.Send(data, 0, data.Length, SocketFlags.None);
                }
                else
                {
                    Err = "Not connected to server yet";
                }
            }
            catch (SocketException se)
            {
                Err = se.Message;
            }
        }

        public void Close()
        {
            if (IsConnected())
            {
                Socket.Close();
            }
            else
            {
                Err = "Socket is already not connected";
            }
        }
    }
}
