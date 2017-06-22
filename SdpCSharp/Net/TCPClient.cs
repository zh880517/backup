using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;

namespace Net
{
    public class TCPClient : TCPConnectPool
    {
        public Action<TCPSession, SocketError> OnConnectResult = (TCPSession token, SocketError e) => { LogHelper.LogInfor("ConnectResult " + e.ToString()); };

        private HashSet<TCPSocketToken> Tokens = new HashSet<TCPSocketToken>();
        public TCPClient(int bufferSize)
        {
            _TokenPool = new TCPSocketTokenPool(1, bufferSize, bufferSize, this);

            OnCloseToken += new EventHandler<TCPSocketToken>((object sender, TCPSocketToken token) =>
            {
                lock(Tokens)
                {
                    Tokens.Remove(token);
                }
            });
        }

        public void Connect(string ip, int port, TCPSession session)
        {
            IPAddress serverIP;
            if (!IPAddress.TryParse(ip, out serverIP))
                serverIP = Dns.GetHostEntry(ip).AddressList[0];
            IPEndPoint endPoint = new IPEndPoint(serverIP, port);
            SocketAsyncEventArgs connectEvent;
            connectEvent = new SocketAsyncEventArgs();
            connectEvent.Completed += new EventHandler<SocketAsyncEventArgs>(OnConnected);
            connectEvent.RemoteEndPoint = endPoint;
            Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            var token = _TokenPool.Pop();
            token.SetSocket(socket);
            session.SetToken(token);
            lock(Tokens)
            {
                Tokens.Add(token);
            }
            connectEvent.UserToken = session;
            if (!socket.ConnectAsync(connectEvent))
                OnConnected(socket, connectEvent);
        }
        
        private void OnConnected(object sender, SocketAsyncEventArgs e)
        {
            TCPSession session = e.UserToken as TCPSession;
            if (e.ConnectSocket != null && e.ConnectSocket.Connected)
            {
                session.Token.DoRecive();
                session.Token.TrySend();
            }
            else
            {
                session.DisConnect();
            }
            OnConnectResult(session, e.SocketError);
        }

        public override void Stop()
        {
            TCPSocketToken[] tokens = new TCPSocketToken[Tokens.Count];
            lock(Tokens)
            {
                Tokens.CopyTo(tokens);
            }
            foreach (var token in tokens)
            {
                token.DisConnect();
            }
        }
        
    }
}
