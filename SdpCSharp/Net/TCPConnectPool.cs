using System;
using System.Collections.Concurrent;
using System.Net.Sockets;

namespace Net
{
    public class TCPConnectPool
    {
        protected TCPSocketTokenPool _TokenPool;

        private object CloseLockObj = new object();
        public event EventHandler<TCPSocketToken> OnCloseToken;

        /// <summary>
        /// 当TCPSocketToken没有绑定TCPSession或者绑定的TCPSession没有设置OnRecivePacket时调用
        /// </summary>
        public Action<TCPSocketToken, RecivePacket> OnRecive = (TCPSocketToken token, RecivePacket packet) => { };
        
        protected uint AllowedMaxPacketLen = 0xFFFFFFFF;

        public uint MaxPacketLen { get { return AllowedMaxPacketLen; } }
        public virtual void Stop()
        {

        }
        

        public void OnIOCompleted(object sender, SocketAsyncEventArgs e)
        {
            TCPSocketToken token = e.UserToken as TCPSocketToken;
            try
            {
                if (e.LastOperation == SocketAsyncOperation.Send)
                {
                    token.OnSendCompleted();
                }
                else if (e.LastOperation == SocketAsyncOperation.Receive && e.BytesTransferred > 0)
                {
                    if (!token.OnReciveCompleted())
                    {
                        LogHelper.LogError("Packet Length is bigger than AllowedMaxPacketLen!!");
                        token.DisConnect();
                        return;
                    }
                    token.DoRecive();
                }
                else
                {
                    if (sender as Socket == token.ConnectSocket)
                        CloseToken(token);
                }

            }
            catch (Exception ex)
            {
                LogHelper.LogException(ex);
                if (sender as Socket == token.ConnectSocket)
                    CloseToken(token);
            }

        }
        
        public void CloseToken(TCPSocketToken token)
        {
            lock (CloseLockObj)
            {
                if (OnCloseToken != null)
                {
                    OnCloseToken(this, token);
                }
                TCPSession session = token.Session;
                if (session != null)
                {
                    session.SetToken(null);
                    session.OnDisconnect();
                }
                _TokenPool.Push(token);
            }
        }
    }
}
