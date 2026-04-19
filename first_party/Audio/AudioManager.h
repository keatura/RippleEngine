#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <string>
#include <algorithm>

struct MIX_Mixer;
struct MIX_Audio;
typedef struct _MonoString MonoString;

inline constexpr float MAX_MASTER_VOLUME = 1.0f;
inline constexpr float MIN_MASTER_VOLUME = 0.0f;

class AudioManager
{
private:
	static inline MIX_Mixer* mixer = nullptr;
	static inline float master_volume = MAX_MASTER_VOLUME;
public:
	static void Initialize();
	static void Shutdown();

	static void PlayOneShot_Wrapper(MonoString* track_name);
	static void PlayOneShot(MIX_Audio* track);

	static MIX_Mixer* GetMixer();
	static inline float GetMasterVolume() { return master_volume; }
	static void SetMasterVolume(float mv);
};

#endif