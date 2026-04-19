#ifndef AUDIOHELPER_H
#define AUDIOHELPER_H

#define AUDIO_HELPER_VERSION 0.8

#include <iostream>
#include <filesystem>

#include "SDL3_mixer/SDL_mixer.h"

class AudioHelper {
public:
	static inline MIX_Mixer* mixer;
	static inline int Mix_PlayChannel(int channel, MIX_Audio* audio, int loops)
	{

		if (!IsAutograderMode())
			return ::MIX_PlayAudio(mixer, audio);
		else
		{
			return channel;
		}
	}

	static inline int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize)
	{
		/*if (!IsAutograderMode())
			return MIX_OpenAudio(frequency, format, channels, chunksize);
		else
			return 0;*/
		return 0;
	}

	static inline int Mix_AllocateChannels(int numchans)
	{
		/*if (!IsAutograderMode())
			return MIX_AllocateChannels(numchans);
		else
			return numchans;*/
		return 0;
	}

	static inline void Mix_Pause(int channel)
	{
		/*std::cout << "(Mix_Pause(" << channel << ") called on frame " << Helper::GetFrameNumber() << ")" << std::endl;

		if (!IsAutograderMode())
			MIX_Pause(channel);*/
	}
	static inline void Mix_Resume(int channel)
	{
		/*std::cout << "(Mix_Resume(" << channel << ") called on frame " << Helper::GetFrameNumber() << ")" << std::endl;

		if (!IsAutograderMode())
			MIX_Resume(channel);*/
	}

	static inline int Mix_HaltChannel(int channel)
	{
		/*std::cout << "(Mix_HaltChannel(" << channel << ") called on frame " << Helper::GetFrameNumber() << ")" << std::endl;

		if (!IsAutograderMode())
			return :IX_HaltChannel(channel);
		else
			return 0;*/
		return 0;
	}

	static inline int Mix_Volume(int channel, int volume)
	{
		/*std::cout << "(Mix_Volume(" << channel << "," << volume << ") called on frame " << Helper::GetFrameNumber() << ")" << std::endl;

		if (!IsAutograderMode())
			return MIX_Volume(channel, volume);
		else
			return 0;*/
		return 0;
	}

	static inline void Mix_CloseAudio(void)
	{
		/*if (!IsAutograderMode())
			MIX_CloseAudio();*/
	}

private:
	static inline MIX_Audio* autograder_dummy_sound;

	/* It's best for everyone if the autograder doesn't actually play any audio (it will still check that you try though). */
	/* Imagine poor Professor Kloosterman hearing the autograder all day as it spams various sounds in my office. */
	static inline bool IsAutograderMode() {
		/* Visual C++ does not like std::getenv */
#ifdef _WIN32
		char* val = nullptr;
		size_t length = 0;
		_dupenv_s(&val, &length, "AUTOGRADER");
		if (val) {
			free(val);
			return true;
		}
		else {
			return false;
		}
#else
		const char* autograder_mode_env_variable = std::getenv("AUTOGRADER");
		if (autograder_mode_env_variable)
			return true;
		else
			return false;
#endif

		return false;
	}
};

#endif
