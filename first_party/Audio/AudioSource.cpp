#include "Audio/AudioSource.h"
#include "Audio/AudioManager.h"
#include "DB/AudioDB.h"
#include "SDL3_mixer/SDL_mixer.h"
void AudioSource::OnStart() {
    MIX_Mixer* mixer = AudioManager::GetMixer();
    track = MIX_CreateTrack(mixer); // might change? unsure if this is wasteful
}

void AudioSource::OnDestroy()
{
    if (track)
        MIX_DestroyTrack(track);
}

void AudioSource::SetClip(const std::string& clip_name) {
    if (clip_name.length() == 0) return; // no clip no problem
    clip = AudioDB::GetAudio(clip_name);
    if (track && clip)
        MIX_SetTrackAudio(track, clip);
}

void AudioSource::Play() {
    if (track && clip) {
        SDL_PropertiesID props = SDL_CreateProperties();
        int num_loops = looping ? -1 : 0;
        SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, num_loops);
        MIX_PlayTrack(track, props);
        SDL_DestroyProperties(props);
    }
}

void AudioSource::Pause() {
    if (track)
        MIX_PauseTrack(track);
}

void AudioSource::Stop() {
    if (track)
        MIX_StopTrack(track, static_cast<Sint64>(fadeout_frames));
}

float AudioSource::GetLocalVolume() const {
    return volume;
}

float AudioSource::GetGlobalVolume() const {
    return volume * AudioManager::GetMasterVolume();
}

void AudioSource::SetVolume(float vol)
{
    this->volume = vol;
    RefreshVolume();
}

void AudioSource::SetLooping(bool loop) {
    looping = loop;

    if (track && loop)
        MIX_SetTrackLoops(track, -1);
    if (track && !loop)
        MIX_SetTrackLoops(track, 0);
}

void AudioSource::SetPlayOnStart(bool play)
{
    play_on_start = play;
}

bool AudioSource::IsPlaying() const
{
    if (!track) return false;
    return MIX_TrackPlaying(track);
}

void AudioSource::RefreshAllVolumes()
{
    size_t num_sources = source_storage.size();
    for (size_t i = 0; i < num_sources; i++) {
        source_storage[i]->RefreshVolume();
    }
}

void AudioSource::RefreshVolume()
{
    if (track)
        MIX_SetTrackGain(track, volume * AudioManager::GetMasterVolume());
}

void AudioSource::SetFadeoutFrames(int f_frames) { // this might only take effect if its not already fading out so just beware :D
    fadeout_frames = f_frames;
}

// csharp stuff
#define DllExport __declspec(dllexport)
extern "C" {
    DllExport void* AS_CreateAudioSource(
        const char* init_clip_name,
        float init_volume,
        bool looping,
        bool play_on_start,
        int fadeout_frames
    ) {
        AudioSource::source_storage.push_back(std::make_unique<AudioSource>());
        AudioSource* new_as = AudioSource::source_storage.back().get();

        new_as->OnStart();

        new_as->SetClip(init_clip_name);
        new_as->SetVolume(init_volume);
        new_as->SetLooping(looping);
        new_as->SetPlayOnStart(play_on_start);
        new_as->SetFadeoutFrames(fadeout_frames);

        if (play_on_start)
            new_as->Play();

        return new_as;
    }

    DllExport void AS_SetClip(void* as_ptr, const char* clip_name) {
        if (!as_ptr) return;
        static_cast<AudioSource*>(as_ptr)->SetClip(clip_name);
    }

    DllExport void AS_Play(void* as_ptr) {
        if (!as_ptr) return;
        static_cast<AudioSource*>(as_ptr)->Play();
    }

    DllExport void AS_Pause(void* as_ptr) {
        if (!as_ptr) return;
        static_cast<AudioSource*>(as_ptr)->Pause();
    }

    DllExport void AS_Stop(void* as_ptr) {
        if (!as_ptr) return;
        static_cast<AudioSource*>(as_ptr)->Stop();
    }

    DllExport float AS_GetLocalVolume(void* as_ptr) {
        if (!as_ptr) return DEFAULT_MAX_VOLUME;
        return static_cast<AudioSource*>(as_ptr)->GetLocalVolume();
    }

    DllExport float AS_GetGlobalVolume(void* as_ptr) {
        if (!as_ptr) return DEFAULT_MAX_VOLUME;
        return static_cast<AudioSource*>(as_ptr)->GetGlobalVolume();
    }

    DllExport void AS_SetVolume(void* as_ptr, float vol) {
        if (!as_ptr) return;
        static_cast<AudioSource*>(as_ptr)->SetVolume(vol);
    }

    DllExport bool AS_IsLooping(void* as_ptr) {
        if (!as_ptr) return DEFAULT_LOOPING;
        return static_cast<AudioSource*>(as_ptr)->IsLooping();
    }

    DllExport void AS_SetLooping(void* as_ptr, bool loop) {
        if (!as_ptr) return;
        static_cast<AudioSource*>(as_ptr)->SetLooping(loop);
    }

    DllExport bool AS_GetPlayOnStart(void* as_ptr) {
        if (!as_ptr) return DEFAULT_PLAY_ON_START;
        return static_cast<AudioSource*>(as_ptr)->GetPlayOnStart();
    }
    
    DllExport void AS_SetPlayOnStart(void* as_ptr, bool play) {
        if (!as_ptr) return;
        static_cast<AudioSource*>(as_ptr)->SetPlayOnStart(play);
    }

    DllExport int AS_GetFadeoutFrames(void* as_ptr) {
        if (!as_ptr) return DEFAULT_FADEOUT_FRAMES;
        return static_cast<int>(static_cast<AudioSource*>(as_ptr)->GetFadeoutFrames());
    }

    DllExport void AS_SetFadeoutFrames(void* as_ptr, int f_frames) {
        if (!as_ptr) return;
        static_cast<AudioSource*>(as_ptr)->SetFadeoutFrames(f_frames);
    }

    DllExport bool AS_IsPlaying(void* as_ptr) {
        if (!as_ptr) return false;
        return static_cast<AudioSource*>(as_ptr)->IsPlaying();
    }

    DllExport void AS_DestroyAudioSource(void* as_ptr) {
        if (!as_ptr) return;

        AudioSource* cpp_as = static_cast<AudioSource*>(as_ptr);

        cpp_as->OnDestroy();

        auto& storage = AudioSource::source_storage;
        for (size_t i = 0; i < storage.size(); i++) {
            if (storage[i].get() == cpp_as) {
                if (i != storage.size() - 1) {
                    std::swap(storage[i], storage.back());
                }

                storage.pop_back();
                break;
            }
        }
    }
}