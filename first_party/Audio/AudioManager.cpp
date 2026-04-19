#include "Audio/AudioManager.h"
#include "Audio/AudioSource.h"

#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h"

#include "mono/metadata/object.h"
#include "mono/metadata/appdomain.h"

#include "DB/AudioDB.h"

#include <iostream>

inline constexpr int NUM_CHANNELS = 50;
inline constexpr int HARDCODED_AUDIO_FREQUENCY = 44100;
inline constexpr int HARDCODED_CHANNEL_NUM = 2;
inline constexpr int HARDCODED_CHUNK_SIZE = 2048;

inline constexpr int MIN_VOLUME = 0;
inline constexpr int MAX_VOLUME = 128;

#define DEBUG_MODE

void AudioManager::Initialize() {
	if (!MIX_Init()) {
#ifdef DEBUG_MODE
		std::cout << "[AUDIOMANAGER] ERROR: SDL3mixer failed to initialize, produced error " << SDL_GetError() << "\n";
#endif
		exit(0);
	}

	if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
#ifdef DEBUG_MODE
		std::cout << "[AUDIOMANAGER] ERROR: SDL_INIT_AUDIO failed, produced error " << SDL_GetError() << "\n";
#endif
		exit(0);
	}

	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (!mixer) {
#ifdef DEBUG_MODE
		std::cout << "[AUDIOMANAGER] ERROR: Could not create mixer device, produced error " << SDL_GetError() << "\n";
#endif
		exit(0);
	}
#ifdef DEBUG_MODE
	std::cout << "[AUDIOMANAGER] Audio system initialized successfully!" << "\n";
#endif

	AudioDB::Initialize(mixer);

	mono_add_internal_call("Scripts.AudioManager::PlayOneShot", &AudioManager::PlayOneShot_Wrapper);
	mono_add_internal_call("Scripts.AudioManager::GetMasterVolume", &AudioManager::GetMasterVolume);
	mono_add_internal_call("Scripts.AudioManager::SetMasterVolume", &AudioManager::SetMasterVolume);

	/*luabridge::getGlobalNamespace(ls)
		.beginNamespace("Audio")
		.addFunction("Play", &Play)
		.addFunction("Halt", &Halt)
		.addFunction("SetVolume", &SetVolume)
		.endNamespace();*/
}

MIX_Mixer* AudioManager::GetMixer() { return mixer; }

void AudioManager::SetMasterVolume(float mv)
{
	master_volume = mv;
	master_volume = std::clamp(master_volume, MIN_MASTER_VOLUME, MAX_MASTER_VOLUME);
	AudioSource::RefreshAllVolumes();

}

void AudioManager::PlayOneShot_Wrapper(MonoString* track_name)
{
	char* cstr = mono_string_to_utf8(track_name);
	std::string audio_name = std::string(cstr);
	MIX_Audio* track = AudioDB::GetAudio(audio_name);
	PlayOneShot(track);
#ifdef DEBUG_MODE
	std::cout << "[AUDIOMANAGER] Played oneshot of track " << audio_name << "\n";
#endif
	mono_free(cstr);
}

void AudioManager::PlayOneShot(MIX_Audio* track) {
	MIX_PlayAudio(mixer, track);
}
