using System.Collections.Generic;

namespace Net
{
    public class TCPSocketTokenPool
    {
        private int _SendBufferSize;
        private int _ReciveBufferSize;
        private Queue<TCPSocketToken> _TokenPool;
        private int _UsedTokenNum = 0;
        private TCPConnectPool connectPool;

        public int UseTokenNum { get { return _UsedTokenNum; } }

        public TCPSocketTokenPool(int capacity, int sendBufferSize, int reciveBufferSize, TCPConnectPool connPool)
        {
            _TokenPool = new Queue<TCPSocketToken>(capacity);
            connectPool = connPool;
            _SendBufferSize = sendBufferSize;
            _ReciveBufferSize = reciveBufferSize;

            for (int i=0; i< capacity; ++i)
            {
                _TokenPool.Enqueue(CreatNewToken());
            }
        }

        public TCPSocketToken Pop()
        {
            lock(this)
            {
                TCPSocketToken token = null;
                if (_TokenPool.Count > 0)
                {
                    token = _TokenPool.Dequeue();
                }
                if(token == null)
                    token = CreatNewToken();
                ++_UsedTokenNum;
                token.Clear();
                return token;
            }
        }
        
        public void Push(TCPSocketToken token)
        {
            lock(this)
            {
                if (token != null && !_TokenPool.Contains(token))
                {
                    --_UsedTokenNum;
                    token.Clear();
                    token.Session = null;
                    _TokenPool.Enqueue(token);
                }
            }
        }

        private TCPSocketToken CreatNewToken()
        {
            return new TCPSocketToken(_SendBufferSize, _ReciveBufferSize, connectPool);
        }
    }
}
