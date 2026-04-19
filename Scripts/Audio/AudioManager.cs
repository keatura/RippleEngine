using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Scripts
{
    public static class AudioManager
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void PlayOneShot(string track_name);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern float GetMasterVolume();
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetMasterVolume(float volume);
    }
}
