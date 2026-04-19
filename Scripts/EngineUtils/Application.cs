using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Scripts
{
    public static class Application {
        private static int curr_frame = 0;
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Quit();
        public static int GetFrame() {  return curr_frame; }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void OpenURL(string url);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Sleep(int dur_ms);

        private static void IncrementFrame()
        {
            curr_frame++;
        }
    }
}
