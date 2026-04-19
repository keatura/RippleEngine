#ifndef AUDIODB_H
#define AUDIODB_H

#include <unordered_map>
#include <string>

struct MIX_Mixer;
struct MIX_Audio;

class AudioDB
{ 
private:
	static inline MIX_Mixer* mixer;
	static inline std::unordered_map<std::string, MIX_Audio*> audio_tracks;
public:
	static inline const std::string base_filepath = "resources/audio/";
	static void Initialize(MIX_Mixer* m);
	static MIX_Audio* GetAudio(const std::string& audio_name);
};

#endif
