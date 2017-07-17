using System;
using System.Text;
using System.Threading;
using System.Collections.Concurrent;

namespace Net.Test
{
    public class ShortHead : IPacketHead
    {
        public int HeadLen
        {
            get{ return 2; }
        }

        public int CalDateLen(byte[] buffer)
        {
            return buffer[0] | buffer[1] << 8;
        }
    }


    public class StringNetPacket : INetPacket
    {
        public static readonly string TestStr = "所谓窥镜，乃促人反省之语。然则真能反省者，几人耳。人居镜前，自恃之，自负之，遂不得省。镜非醒悟之器，乃迷惑之器。初见不悟，而再见、三见，渐至迷途。";
        private byte[] Buffer;
        
        public StringNetPacket(string val)
        {
            byte[] buffer = Encoding.Default.GetBytes(val);
            Buffer = new byte[buffer.Length + 2];
            WriteLength(buffer.Length);
        }

        public StringNetPacket(int len)
        {
            Buffer = new byte[len + 2];
            for (int i=2; i<len+2; ++i)
            {
                Buffer[i] = (byte)(i-2);
            }
            WriteLength(len);
        }

        public bool HasHead
        {
            get { return false; }
        }

        public int Length
        {
            get{ return Buffer.Length; }
        }

        public int Offset
        {
            get{ return 0; }
        }
        

        public int Read(byte[] desBuffer, int desOffset, int srcOffset, int length)
        {
            if (srcOffset >= Buffer.Length || srcOffset < 0)
                return 0;

            if (length + desOffset > desBuffer.Length)
                length = desBuffer.Length - desOffset;

            int iReadLen = length;
            if (srcOffset + length > Buffer.Length)
            {
                iReadLen = Buffer.Length - srcOffset;
            }
            Array.Copy(Buffer, srcOffset, desBuffer, desOffset, iReadLen);
            return iReadLen;
        }

        private void WriteLength(int len)
        {
            Buffer[0] = (byte)(len & 0xFF);
            Buffer[1] = (byte)((len >> 8) & 0xFF);
        }
    }
    public class SeverTest
    {
        TCPListener listener = new TCPListener(new ShortHead(),12345, 10, 1024, 1024);
        Thread thread;
        AutoResetEvent mutex = new AutoResetEvent(false);
        private ConcurrentDictionary<TCPSocketToken, TCPSession> clients = new ConcurrentDictionary<TCPSocketToken, TCPSession>();
        public void Start()
        {
            //listener = new TCPListener(12345, 10, 1024, 1024);
            listener.OnRecive = OnRecive;
            thread = new Thread(() => {
                listener.Start();
                mutex.WaitOne();
            });
            listener.OnCloseToken += new EventHandler<TCPSocketToken>(CloseToken);
            thread.Start();
        }

        public void Stop()
        {
            mutex.Set();
            thread.Join();
            listener.Stop();
        }

        private void OnRecive(TCPSocketToken token, RecivePacket packet)
        {
            TCPSession sendList = clients.GetOrAdd(token, (TCPSocketToken tk) => 
            {
                TCPSession session = new TCPSession();
                session.SetToken(tk);
                return session;
            });
            /*
            string str = Encoding.Default.GetString(pack.Buffer, pack.HeadLen, pack.Length - pack.HeadLen);
            if (str != StringNetPacket.TestStr)
            {
                Console.WriteLine("server recive error !");
            }
            */
            for (int i = 0; i < packet.Length - packet.HeadLen; ++i)
            {
                if (packet.Buffer[packet.HeadLen + i] != (byte)i)
                {
                    Console.WriteLine("!!!");
                }
            }
            StringNetPacket pongPacket = new StringNetPacket(1024);
            token.Session.SendPacket(pongPacket);
        }

        private void CloseToken(object sender, TCPSocketToken token)
        {
            TCPSession sesion;
            clients.TryRemove(token, out sesion);
        }
    }
}
