#include "Utils/Application.h"

#include <thread>
#include <chrono>
#include <iostream>

#include "mono/metadata/object.h"
#include "mono/metadata/appdomain.h"

void Application::Quit()
{
    exit(0);
}

int Application::GetFrame()
{
    return frame_num;
}

void Application::IncrementFrame() {
    frame_num++;
    MonoObject* ex = nullptr;
    increment_frame_ptr(&ex);
}

void Application::OpenURL_Wrapper(MonoString* mono_url) {
    char* cstr = mono_string_to_utf8(mono_url);
    std::string cpp_str(cstr);
    mono_free(cstr);
    Application::OpenURL(cpp_str);
}

void Application::OpenURL(std::string url)
{
    std::string to_use = "";
#ifdef _WIN32
    to_use = "start " + url;
#elif __APPLE__
    to_use = "open " + url;
#else
    to_use = "xdg-open " + url;
#endif
    std::system(to_use.c_str());
}

void Application::Sleep(int dur_ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(dur_ms));
}

void Application::Initialize(MonoImage* image) {
    MonoClass* application_class = mono_class_from_name(image, "Scripts", "Application");
    if (!application_class) {
        std::cout << "Error: application class missing" << "\n";
        exit(0);
    }
    increment_frame_method = mono_class_get_method_from_name(application_class, "IncrementFrame", 0);
    if (!increment_frame_method) {
        std::cout << "Error: couldn't find increment frame method in application class" << "\n";
        exit(0);
    }

    increment_frame_ptr = (IncrementFrameThunk)mono_method_get_unmanaged_thunk(increment_frame_method);

    mono_add_internal_call("Scripts.Application::Quit", &Application::Quit); // TODO
    mono_add_internal_call("Scripts.Application::Sleep", &Application::Sleep);
    mono_add_internal_call("Scripts.Application::OpenURL", &Application::OpenURL_Wrapper);
}