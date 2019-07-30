using System;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Collections.Concurrent;

namespace echoClient_csharp
{
    public enum LOG_LEVEL { DISABLE, INFO, WARN, ERROR, TRACE, DEBUG }
    public class Log
    {
        private static ConcurrentQueue<string> LogMsgQueue = null;
        private static long LogLevel = (long)LOG_LEVEL.INFO;

        public static void Init(LOG_LEVEL logLevel)
        {
            ChangeLogLevel(logLevel);
            LogMsgQueue = new ConcurrentQueue<string>();
        }

        public static void ChangeLogLevel(LOG_LEVEL logLevel)
        {
            Interlocked.Exchange(ref LogLevel, (long)logLevel);
        }

        public static LOG_LEVEL CurrentLogLevel()
        {
            return (LOG_LEVEL)Interlocked.Read(ref LogLevel);
        }

        public static void Write(string msg, LOG_LEVEL logLevel = LOG_LEVEL.TRACE,
                                [CallerMemberName] string methodName = "",
                                [CallerFilePath] string filePath = "",
                                [CallerLineNumber] int lineNumber = 0)
        {
            if (CurrentLogLevel() <= logLevel)
            {
                string logMsg = string.Format("{0}| {1}", DateTime.Now, msg);
                LogMsgQueue.Enqueue(logMsg);
            }
        }

        static public bool GetLog(out string msg)
        {
            if (LogMsgQueue.TryDequeue(out msg))
            {
                return true;
            }
            return false;
        }
    }
}
