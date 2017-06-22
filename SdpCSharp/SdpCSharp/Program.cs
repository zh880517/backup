using System;
using System.Collections.Generic;

namespace Sdp
{
    [Serializable]
    partial class TestSdp : IStruct
    {
        [NonSerialized]
        private static readonly string[] _member_name_ = new string[] { "iValue", "strValue" , "Values" , "KeyValues" };

        public int iValue;
        public string strValue;
        public List<int> Values = new List<int>();
        public Dictionary<int, DateTime> KeyValues = new Dictionary<int, DateTime>();

        public void Visit(ISdp sdp)
        {
            sdp.Visit(0, _member_name_[0], false, ref iValue);
            sdp.Visit(1, _member_name_[1], false, ref strValue);
            sdp.Visit(2, _member_name_[2], false, Values);
            sdp.Visit(233, _member_name_[3], false, KeyValues);
        }
        
    }
    
    class Program
    {
        static void TestT<T>(T val)
        {
            Type type = typeof(T);

            foreach (var it in type.GetInterfaces())
            {
                if (it == typeof(System.Collections.IDictionary))
                {
                    Type[] genericTypes = type.GetGenericArguments();
                    Console.WriteLine(genericTypes[0].Name + " " + genericTypes[1].Name);
                }
                else if (it == typeof(System.Collections.IList))
                {
                    Type[] genericTypes = type.GetGenericArguments();
                    Console.WriteLine(genericTypes[0].Name );
                }
            }
        }
        
        enum eTest
        {
            eFirst = 0,
            eSecond,
        }

        static DateTime CalNextWeekDay(DateTime now, int week, int hour)
        {
            week = week % 7;
            hour = hour % 24;
            var today = new DateTime(now.Year, now.Month, now.Day);
            DateTime time = now;
            if (today.DayOfWeek == (DayOfWeek)week)
            {
                time = today.AddDays(7);
            }
            else if (today.DayOfWeek < (DayOfWeek)week)
            {
                time = today.AddDays((DayOfWeek)week - today.DayOfWeek);
            }
            else
            {
                time = today.AddDays(7 + week - (int)today.DayOfWeek);
            }
            time = time.AddHours(hour);
            return time;
        }
        
        static void Main(string[] args)
        {
            TestSdp sdp = new TestSdp();
            sdp.Values.Add(1);
            sdp.iValue = 127;
            sdp.strValue = "1";
            sdp.KeyValues.Add(1, DateTime.Now);
            TestSdp sdp2 = new TestSdp();
            sdp2.Deserialize(Sdp.Serializer(sdp));

            //List<int> list = new List<int>() { 1, 21, 3, 4 };
            //List<int> ls2 = Sdp.Deserialize<List<int>>(Sdp.Serializer(list));

            //DateTime now = DateTime.Parse("2017/3/19");
            //Console.WriteLine(CalNextWeekDay(now, 7, 24));

            //List<eTest> list = new List<eTest>() { eTest.eFirst, eTest.eSecond };
            //List<eTest> ls2 = Sdp.Deserialize<List<eTest>>(Sdp.Serializer(list));
            /*
            eTest test = (eTest)0x2106;
            Console.WriteLine(test.ToString());
            Console.ReadKey();
            */
            Console.ReadKey();
        }
    }
}
