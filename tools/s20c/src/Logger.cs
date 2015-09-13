using System;

namespace cc
{
    public static class Logger
    {
        private static readonly object LogLock = new object();

        public static void WriteLine(string format, params object[] arg)
        {
            lock (LogLock)
            {
                Console.ForegroundColor = ConsoleColor.Gray;
                Console.WriteLine(format, arg);
                Console.ForegroundColor = ConsoleColor.Gray;
            }
        }

        public static void WriteLine(ConsoleColor color, string format, params object[] arg)
        {
            lock (LogLock)
            {
                Console.ForegroundColor = color;
                Console.WriteLine(format, arg);
                Console.ForegroundColor = ConsoleColor.Gray;
            }
        }
    }
}
