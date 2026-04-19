using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Scripts
{
    public class Debug
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Log(string msg);

        public static void Log(int msg)
        {
            Debug.Log(msg.ToString());
        }

        public static void Log(float msg)
        {
            Debug.Log(msg.ToString());
        }
    }
}
