using Scripts.Audio;
using Scripts.ECS;
using System;
using System.Collections.Generic;
using System.Text;

namespace Scripts
{
    public struct AudioSource : IComponent, IHasStart, IHasDestroy
    {
        public string clip_name = "";
        public float init_volume = 1.0f;
        public bool looping = false;
        public bool play_on_start = false;
        public int fadeout_frames = 0;

        private IntPtr native_as_ptr = IntPtr.Zero;

        public AudioSource() { }

        public void OnStart(int self_actor_id)
        {
            native_as_ptr = AudioInterop.AS_CreateAudioSource(clip_name, init_volume, looping, play_on_start, fadeout_frames);
        }

        public void OnDestroy(int self_actor_id)
        {
            if (native_as_ptr == IntPtr.Zero) return;
            AudioInterop.AS_DestroyAudioSource(native_as_ptr);
        }

        public void SetClip(string cn)
        {
            if (native_as_ptr == IntPtr.Zero)
            {
                clip_name = cn;
                return;
            }
            AudioInterop.AS_SetClip(native_as_ptr, cn);
        }

        public void Play()
        {
            if (native_as_ptr == IntPtr.Zero) return;
            AudioInterop.AS_Play(native_as_ptr);
        }

        public void Pause()
        {
            if (native_as_ptr == IntPtr.Zero) return;
            AudioInterop.AS_Pause(native_as_ptr);
        }

        public void Stop()
        {
            if (native_as_ptr == IntPtr.Zero) return;
            AudioInterop.AS_Stop(native_as_ptr);
        }

        public float GetLocalVolume()
        {
            if (native_as_ptr == IntPtr.Zero) return init_volume;
            return AudioInterop.AS_GetLocalVolume(native_as_ptr);
        }

        public float GetGlobalVolume()
        {
            if (native_as_ptr == IntPtr.Zero) return init_volume * AudioManager.GetMasterVolume();
            return AudioInterop.AS_GetGlobalVolume(native_as_ptr);
        }

        public void SetVolume(float volume)
        {
            if (native_as_ptr == IntPtr.Zero)
            {
                init_volume = volume;
                return;
            }
            AudioInterop.AS_SetVolume(native_as_ptr, volume);
        }

        public bool IsLooping()
        {
            if (native_as_ptr == IntPtr.Zero) return looping;
            return AudioInterop.AS_IsLooping(native_as_ptr);
        }

        public void SetLooping(bool loop)
        {
            if (native_as_ptr == IntPtr.Zero)
            {
                looping = loop;
                return;
            }
            AudioInterop.AS_SetLooping(native_as_ptr, loop);
        }

        public bool GetPlayOnStart()
        {
            if (native_as_ptr == IntPtr.Zero) return play_on_start;
            return AudioInterop.AS_GetPlayOnStart(native_as_ptr);
        }

        public void SetPlayOnStart(bool play)
        {
            if (native_as_ptr == IntPtr.Zero)
            {
                play_on_start = play;
                return;
            }
            AudioInterop.AS_SetPlayOnStart(native_as_ptr, play);
        }

        public int GetFadeoutFrames()
        {
            if (native_as_ptr == IntPtr.Zero) return fadeout_frames;
            return AudioInterop.AS_GetFadeoutFrames(native_as_ptr);
        }

        public void SetFadeoutFrames(int f_frames)
        {
            if (native_as_ptr == IntPtr.Zero)
            {
                fadeout_frames = f_frames;
                return;
            }
            AudioInterop.AS_SetFadeoutFrames(native_as_ptr, f_frames);
        }

        public bool IsPlaying()
        {
            if (native_as_ptr == IntPtr.Zero) return false;
            return AudioInterop.AS_IsPlaying(native_as_ptr);
        }
    }
}
