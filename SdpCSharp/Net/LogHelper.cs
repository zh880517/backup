using System;

namespace Net
{
    public interface ILoger
    {
        void LogInfor(string log);
        void LogError(string log);
        void LogException(Exception ex);
    }

    class ConsoleLog : ILoger
    {
        public void LogError(string log)
        {
            Console.WriteLine("Error : " + log);
        }

        public void LogException(Exception ex)
        {
            Console.WriteLine(ex.Message);
            Console.WriteLine(ex.StackTrace);
        }

        public void LogInfor(string log)
        {
            Console.WriteLine("Infor : " + log);
        }
    }

    public static class LogHelper
    {
        private static ILoger Loger = new ConsoleLog();

        public static void SetLoger(ILoger loger)
        {
            Loger = loger;
        }

        public static void LogInfor(string log)
        {
            if (Loger != null)
            {
                Loger.LogInfor(log);
            }
        }

        public static void LogError(string log)
        {
            if (Loger != null)
            {
                Loger.LogError(log);
            }
        }
        public static void LogException(Exception ex)
        {
            if (Loger != null)
            {
                Loger.LogException(ex);
            }
        }

        public static void LogDebugException(Exception ex)
        {
            if (Loger != null)
            {
                Loger.LogException(ex);
            }
        }
    }
}
