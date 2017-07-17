using System;
using System.Net;
using System.Net.Sockets;

namespace Net
{
    public class TCPListener : TCPConnectPool
    {
        private Socket _LocalSocket;
        private IPEndPoint _LocalEndPoint;
        private SocketAsyncEventArgs _AcceptEvent;
        
        public TCPListener(IPacketHead head, int port, int numConnections, int receiveBufferSize, int sendBufferSize, int allowedMaxPacketLen = -1)
        {
            _PackHead = head;
            _TokenPool = new TCPSocketTokenPool(numConnections, sendBufferSize, receiveBufferSize, this);

            _LocalEndPoint = new IPEndPoint(IPAddress.Any, port);
            if (allowedMaxPacketLen > 0)
            {
                AllowedMaxPacketLen = (uint)allowedMaxPacketLen;
            }
        }

        public void Start()
        {
            if (_LocalSocket != null || _AcceptEvent != null)
                Stop();

            _LocalSocket = new Socket(_LocalEndPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            _LocalSocket.Bind(_LocalEndPoint);
            _LocalSocket.Listen(100);
            _LocalSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.KeepAlive, true);
            _LocalSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.DontLinger, true);
            _AcceptEvent = new SocketAsyncEventArgs();
            _AcceptEvent.Completed += new EventHandler<SocketAsyncEventArgs>(OnAcceptCompleted);
            if (!_LocalSocket.AcceptAsync(_AcceptEvent))
                ProcessAccept(_AcceptEvent);

            Console.WriteLine("Start Listen " + _LocalEndPoint.ToString());
        }

        public override void Stop()
        {
            if (_AcceptEvent != null)
            {
                _AcceptEvent.Completed -= new EventHandler<SocketAsyncEventArgs>(OnAcceptCompleted);
                _AcceptEvent.Dispose();
                _AcceptEvent = null;
            }
            if (_LocalSocket != null)
            {
                _LocalSocket.Close();
                _LocalSocket = null;
            }
        }

        private void OnAcceptCompleted(object sender, SocketAsyncEventArgs e)
        {
            ProcessAccept(e);
        }

        private void ProcessAccept(SocketAsyncEventArgs e)
        {
            bool bLoop = false;
            do 
            {
                Socket socket = null;
                try
                {
                    if (e.SocketError != SocketError.Success)
                    {
                        var errorCode = (int)e.SocketError;
                        if (errorCode == 995 || errorCode == 10004 || errorCode == 10038)
                            return;
                    }
                    socket = e.AcceptSocket;
                    if (socket != null)
                        OnNewSocket(socket);
                }
                catch (Exception ex)
                {
                    LogHelper.LogException(ex);
                    if (socket != null)
                        socket.Close();
                }
                finally
                {
                    _AcceptEvent.AcceptSocket = null;
                    bLoop = !_LocalSocket.AcceptAsync(_AcceptEvent);
                }
            } while (bLoop);
        }

        private void OnNewSocket(Socket socket)
        {
            var token = _TokenPool.Pop();
            token.SetSocket(socket);
            token.DoRecive();
        }
        
    }
}
