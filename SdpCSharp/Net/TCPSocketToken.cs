using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Threading;

namespace Net
{
    
    public class TCPSocketToken
    {
        
        protected Socket remoteSocket;
        protected SocketAsyncEventArgs sendSAEA;
        protected SocketAsyncEventArgs reciveSAEA;
        private TCPConnectPool connnectPool;

        protected INetPacket currSendPacket;
        protected WriteBuffer writeBuffer = new WriteBuffer();

        protected ConcurrentQueue<INetPacket> SendList = null;

        //包头
        protected NetPacketHead currPackHead = new NetPacketHead();
        protected ReadBuffer readBuffer = new ReadBuffer();

        //当前正在接受的包
        protected RecivePacket recivePacket = null;

        private TCPSession session = null;

        public TCPSession Session
        {
            get { return Interlocked.CompareExchange(ref session, session, null); }
            set
            {
                Interlocked.Exchange(ref session, value);
                if (value == null)
                {
                    SendList = null;
                }
            }
        }
        public Socket ConnectSocket
        {
            get { return remoteSocket; }
        }

        public TCPSocketToken(int sendBufferSize, int reciveBufferSize, TCPConnectPool conPool)
        {
            connnectPool = conPool;
            byte[] sendBuffer = new byte[sendBufferSize];
            sendSAEA = new SocketAsyncEventArgs();
            sendSAEA.SetBuffer(sendBuffer, 0, sendBuffer.Length);
            sendSAEA.UserToken = this;
            sendSAEA.Completed += new EventHandler<SocketAsyncEventArgs>(conPool.OnIOCompleted);

            byte[] reciveBuffer = new byte[reciveBufferSize];
            reciveSAEA = new SocketAsyncEventArgs();
            reciveSAEA.SetBuffer(reciveBuffer, 0, reciveBuffer.Length);
            reciveSAEA.UserToken = this;
            reciveSAEA.Completed += new EventHandler<SocketAsyncEventArgs>(conPool.OnIOCompleted);
        }

        public bool IsConnect { get { return remoteSocket != null && remoteSocket.Connected; } }
        
        public void SetSocket(Socket socket)
        {
            remoteSocket = socket;
            sendSAEA.AcceptSocket = socket;
            reciveSAEA.AcceptSocket = socket;
            
        }
        public void DisConnect()
        {
            lock (this)
            {
                if (remoteSocket != null)
                    connnectPool.CloseToken(this);
            }
        }

        public void Clear()
        {
            lock(this)
            {
                Close();
                SetSocket(null);
                SendList = null;
                currSendPacket = null;
                writeBuffer.SetPacket(null);
                recivePacket = null;
                currPackHead.Reset();
            }
        }

        public void SetPacketList(ConcurrentQueue<INetPacket> packlist)
        {
            SendList = packlist;
        }

        public void OnSendCompleted()
        {
            lock(this)
            {
                if (currSendPacket != null && SendList != null)
                {
                    if (writeBuffer.IsComplete())
                    {
                        INetPacket pack = null;
                        if (SendList.TryPeek(out pack) && pack == currSendPacket)
                            SendList.TryDequeue(out pack);
                        currSendPacket = null;
                    }
                }
                Send();
            }
        }

        public bool TrySend()
        {
            lock (this)
            {
                if (currSendPacket != null)
                    return false;
                Send();
                return true;
            }
        }

        public bool OnReciveCompleted()
        {
            readBuffer.Set(reciveSAEA.Buffer, reciveSAEA.Offset, reciveSAEA.BytesTransferred);
            while (!readBuffer.IsOver)
            {
                if (recivePacket == null)
                {
                    //读取包长度
                    if (!currPackHead.Write(readBuffer))
                        break;
                    if ((uint)currPackHead.GetPackLen() > connnectPool.MaxPacketLen)
                        return false;
                    recivePacket = new RecivePacket(currPackHead);
                    currPackHead.Reset();
                }
                else
                {
                    //读取包内容
                    recivePacket.Write(readBuffer);
                    RecivePacket packet = recivePacket;
                    if (packet != null && packet.IsWriteFull)
                    {
                        if (session != null && session.OnRecivePacket != null)
                        {
                            session.OnRecivePacket(session, packet);
                        }
                        else
                        {
                            connnectPool.OnRecive(this, packet);
                        }
                        recivePacket = null;
                    }
                }
            }
            return true;
        }
        
        public void DoRecive()
        {
            bool bComplete = false;

            if (remoteSocket != null && remoteSocket.Connected)
                bComplete = !remoteSocket.ReceiveAsync(reciveSAEA);

            if (bComplete)
                connnectPool.OnIOCompleted(remoteSocket, reciveSAEA);
        }

        protected void Send()
        {
            while (currSendPacket == null && SendList != null)
            {
                if (!SendList.TryPeek(out currSendPacket) || currSendPacket != null)
                    break;
                if (currSendPacket == null)
                    SendList.TryDequeue(out currSendPacket);
            }
            if (currSendPacket == null)
                return;
            writeBuffer.SetPacket(currSendPacket);
            writeBuffer.Write(sendSAEA);
            bool bComplete = false;

            if (remoteSocket != null && remoteSocket.Connected)
            {
                bComplete = !remoteSocket.SendAsync(sendSAEA);
            }

            if (bComplete)
                connnectPool.OnIOCompleted(remoteSocket, sendSAEA);
        }
        
        private void Close()
        {
            if (remoteSocket != null)
            {
                if (remoteSocket.Connected)
                    remoteSocket.Shutdown(SocketShutdown.Both);
                remoteSocket.Close();
            }
        }

    }
}
