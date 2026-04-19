#pragma once

#include <string>

typedef struct _MonoString MonoString;
typedef struct _MonoObject MonoObject;
typedef struct _MonoImage MonoImage;
typedef struct _MonoMethod MonoMethod;

typedef void (*IncrementFrameThunk)(MonoObject**);

class Application {
private:
	static void OpenURL_Wrapper(MonoString* mono_url);
	static inline IncrementFrameThunk increment_frame_ptr = nullptr;
	static inline MonoMethod* increment_frame_method = nullptr;
public:
	static inline size_t frame_num = 0;
	static void Quit();
	static int GetFrame();
	static void IncrementFrame();
	static void OpenURL(std::string url);
	static void Sleep(int dur_ms);
	static void Initialize(MonoImage* img);
};