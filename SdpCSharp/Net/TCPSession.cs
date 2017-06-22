using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading;

namespace Net
{
    public class TCPSession
    {
        protected TCPSocketToken _Token;
        protected ConcurrentQueue<INetPacket> _SendList = new ConcurrentQueue<INetPacket>();

        public Action OnDisconnect = () => { };

        /// <summary>
        /// 处理接收到的数据包，若不设置，则启用TCPConnectPool的OnRecive
        /// </summary>
        public Action<TCPSession, RecivePacket> OnRecivePacket;
        public TCPSocketToken Token
        {
            get { return Interlocked.CompareExchange(ref _Token, _Token, null); }
        }
        
        /// <summary>
        /// 用户数据，方便根据TcpSession反推接受消息对象，客户端基本没用
        /// </summary>
        public object UserData { get; set; }

        public int PackCount { get { return _SendList.Count; } }

        public bool IsConnect()
        {
            TCPSocketToken token = Token;
            return token != null && token.IsConnect;
        }

        public virtual bool SendPacket(INetPacket packt)
        {
            _SendList.Enqueue(packt);
            TCPSocketToken token = Token;
            if (token == null || !token.IsConnect)
                return false;
            token.TrySend();
            return true;
        }

        public bool SendPackets(IEnumerable<INetPacket> packets)
        {
            foreach (var pack in packets)
            {
                _SendList.Enqueue(pack);
            }
            TCPSocketToken token = Token;
            if (token == null || !token.IsConnect)
                return false;
            token.TrySend();
            return true;
        }

        public void SetToken(TCPSocketToken token)
        {
            Interlocked.Exchange(ref _Token, token); 
            if (token != null)
            {
                token.Session = this;
                token.SetPacketList(_SendList);
            }
        }
        
        public void DisConnect()
        {
            if (Token != null )
            {
                Token.DisConnect();
            }
        }
    }
}
