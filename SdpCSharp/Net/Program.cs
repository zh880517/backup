using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
namespace Net
{
    class Program
    {
        static List<string> test (params string[] param)
        {
            return new List<string>(param);
        }
        static void Main(string[] args)
        {
            Test.SeverTest server = new Test.SeverTest();
            
            Test.ClientTest client = new Test.ClientTest();
            for (int j=0; j<3; ++j)
            {
                //server.Start();
                for (int i = 0; i < 10; ++i)
                {
                    client.Start();
                    client.Stop();
                    Console.WriteLine("One turn end!");
                }
                //server.Stop();
                //Console.WriteLine("server.Stop!");
            }
            Console.WriteLine("end!!!");
            
            //server.Start();
            Console.ReadKey();
            //server.Stop();
        }
    }
}
