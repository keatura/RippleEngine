#include "DB/AudioDB.h"
#include "Audio/AudioHelper.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h" 

#include <filesystem>
#include <iostream>

void AudioDB::Initialize(MIX_Mixer* m) {
	AudioDB::mixer = m;

	if (std::filesystem::exists(base_filepath)) {
		for (const auto& audio_file : std::filesystem::directory_iterator(base_filepath)) {
			if (audio_file.is_regular_file()) {
				auto ext = audio_file.path().extension().string(); // the extension
				if (ext == ".wav" || ext == ".ogg") {
					std::string name = (audio_file.path().stem().string());
					MIX_Audio* curr_audio = MIX_LoadAudio(mixer, audio_file.path().string().c_str(), true);
					audio_tracks[name] = curr_audio;
				}
			}
		}
	}
}

MIX_Audio* AudioDB::GetAudio(const std::string& audio_name)
{
	if (audio_name.empty()) return nullptr;
	if (audio_tracks.find(audio_name) == audio_tracks.end()) {
		if (!std::filesystem::exists(base_filepath + audio_name + ".ogg") && !std::filesystem::exists(base_filepath + audio_name + ".wav")) {
			std::cout << "error: failed to play audio clip " << audio_name;
			exit(0);
		}
	}
	return audio_tracks[audio_name];
}
