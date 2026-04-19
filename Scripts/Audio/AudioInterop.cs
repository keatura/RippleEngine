using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Scripts.Audio
{
    public static class AudioInterop {
        private const string DllName = "game_engine.exe";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr AS_CreateAudioSource(
            string init_clip_name,
            float init_volume,
            bool looping,
            bool play_on_start,
            int fadeout_frames
            );
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AS_SetClip(IntPtr as_ptr, string clip_name);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AS_Play(IntPtr as_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AS_Pause(IntPtr as_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AS_Stop(IntPtr as_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern float AS_GetLocalVolume(IntPtr as_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern float AS_GetGlobalVolume(IntPtr as_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AS_SetVolume(IntPtr as_ptr, float vol);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool AS_IsLooping(IntPtr as_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AS_SetLooping(IntPtr as_ptr, bool loop);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool AS_GetPlayOnStart(IntPtr as_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AS_SetPlayOnStart(IntPtr as_ptr, bool play);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int AS_GetFadeoutFrames(IntPtr as_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AS_SetFadeoutFrames(IntPtr as_ptr, int f_frames);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool AS_IsPlaying(IntPtr as_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AS_DestroyAudioSource(IntPtr as_ptr);
    }
}
