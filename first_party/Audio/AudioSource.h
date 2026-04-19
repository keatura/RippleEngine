#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <vector>

struct MIX_Track;
struct MIX_Audio;

inline constexpr float DEFAULT_MAX_VOLUME = 1.0f;
inline constexpr bool DEFAULT_LOOPING = false;
inline constexpr bool DEFAULT_PLAY_ON_START = false;
inline constexpr int64_t DEFAULT_FADEOUT_FRAMES = 0;

// audio source component
class AudioSource
{
private:
    int64_t fadeout_frames = DEFAULT_FADEOUT_FRAMES;
    MIX_Track* track = nullptr;
    MIX_Audio* clip = nullptr;
    float volume = DEFAULT_MAX_VOLUME;
    bool looping = DEFAULT_LOOPING;
    bool play_on_start = DEFAULT_PLAY_ON_START;

public:
    static inline std::vector<std::unique_ptr<AudioSource>> source_storage;
    void OnStart();
    void OnDestroy();

    void SetClip(const std::string& clip_name);
    void Play();
    void Pause();
    void Stop();

    static void RefreshAllVolumes();
    void RefreshVolume();

    float GetLocalVolume() const;
    float GetGlobalVolume() const;
    void SetVolume(float vol);

    inline bool IsLooping() const { return looping; }
    void SetLooping(bool loop);

    inline bool GetPlayOnStart() const { return play_on_start; }
    void SetPlayOnStart(bool play);

    inline int64_t GetFadeoutFrames() { return fadeout_frames; }
    void SetFadeoutFrames(int f_frames);

    bool IsPlaying() const;
};

