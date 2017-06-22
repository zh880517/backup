using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RealTimeRank
{
    class TestRank : RealTimeRank<long>
    {
        protected override IRankUnit<long> CreateUnit(long key, long initValue)
        {
            return new TestRankUnit() { ID = key, Value = initValue };
        }
    }

    public class CRegist
    {

    }

    public interface ITestA
    {
        void A();
    }

    public interface ITestB
    {
        void B();
    }

    public class Entity : ITestB, ITestA
    {
        public void A()
        {
            throw new NotImplementedException();
        }

        public void B()
        {
            throw new NotImplementedException();
        }
    }

    public static class Helper
    {
        public static void Regist(this CRegist regist, ITestA A)
        {
            Console.WriteLine("AAAA!");
        }

        public static void Regist(this CRegist regist, ITestB B)
        {
            Console.WriteLine("BBBB!");
        }
    }

    class Program
    {

        static void Main(string[] args)
        {
            for (int i =0; i< 10; ++i)
            {
                long tick = DateTime.Now.Ticks;
                Console.WriteLine(tick);
            }
            var rank = new TestRank();
            var random = new Random();
            for (int i=0; i<1000; ++i)
            {
                rank.AddValue(random.Next(500), random.Next(10000));
            }
            for (int i = 0; i < 1000; ++i)
            {
                rank.SetValue(random.Next(500), random.Next(10000));
            }
            rank.Check();
        }
    }
}
