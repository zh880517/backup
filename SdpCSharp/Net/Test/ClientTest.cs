using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Net.Test
{
    class SendInfor
    {
        public int SendNum;
        public int Index;
        public int times;
    }
    public class ClientTest
    {
        TCPClient client = new TCPClient(1024);
        const int MaxNum = 100;
        List<TCPSession> Sessions = new List<TCPSession>(MaxNum);
        int times = 0;

        AutoResetEvent waiteEvent = new AutoResetEvent(false);
        int count;
        public ClientTest()
        {
            client.OnConnectResult = OnConnect;
            client.OnRecive = OnRecive;
            client.OnCloseToken += new EventHandler<TCPSocketToken>(OnClose);
        }

        public void Start()
        {
            times++;
            count = 0;
            for (int i=0; i<MaxNum; ++i)
            {
                TCPSession session = new TCPSession();
                lock(Sessions)
                {
                    Sessions.Add(session);
                }
                SendInfor infor = new SendInfor() { Index = i, times = times};
                session.UserData = infor;
                client.Connect("127.0.0.1", 12345, session);
                Thread.Sleep(2);
            }
        }

        public void Stop()
        {
            waiteEvent.WaitOne();
            foreach (var session in Sessions)
            {
                SendInfor infor = session.UserData as SendInfor;
                if (infor.SendNum < 100)
                {
                    Console.WriteLine(string.Format("{0}, {1}, {2}", infor.Index, session.PackCount, infor.SendNum));
                }
            }
            client.Stop();
        }

        private void OnConnect(TCPSession session, SocketError e)
        {
            if (e != SocketError.Success)
            {
                Console.WriteLine(string.Format("{0} : ConnectResult {1} " , (session.UserData as SendInfor).Index, e.ToString()));
                return;
            }
            SendInfor infor = session.UserData as SendInfor;
            SendPack(session);
        }

        private void OnRecive(TCPSocketToken token, RecivePacket packet)
        {
            /*
            string str = Encoding.Default.GetString(packet.Buffer, packet.HeadLen, packet.Length - packet.HeadLen);
            if (str != StringNetPacket.TestStr)
            {
                Console.WriteLine("client recive error!" + str);
            }
            */
            for (int i=0; i<packet.Length - packet.HeadLen; ++i)
            {
                if(packet.Buffer[packet.HeadLen + i] != (byte)i)
                {
                    Console.WriteLine("!!!");
                }
            }
            SendPack(token.Session);
        }

        private void SendPack(TCPSession session)
        {
            SendInfor infor = session.UserData as SendInfor;
            if (infor.SendNum > 100)
            {
                session.DisConnect();
                return;
            }
            StringNetPacket pack = new StringNetPacket(StringNetPacket.TestStr);
            infor.SendNum++;
            session.SendPacket(pack);
        }

        private void OnClose(object sender, TCPSocketToken token)
        {
            lock(Sessions)
            {
                var session = Sessions.FirstOrDefault(obj => obj.Token == token);
                if (session != null)
                {
                    SendInfor infor = session.UserData as SendInfor;
                    if (infor.SendNum < 100)
                    {
                        Console.WriteLine(string.Format("On Close: {0}, {1}, {2}", infor.Index, session.PackCount, infor.SendNum));
                    }
                }
                count++;
                Sessions.Remove(session);
                //Console.WriteLine(count);
                if (count == MaxNum)
                    waiteEvent.Set();
            }
        }
    }
}
