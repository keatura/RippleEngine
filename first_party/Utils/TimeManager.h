#pragma once

#include <chrono>
#include <iostream>

#include "mono/metadata/object.h"
#include "mono/metadata/appdomain.h"

class TimeManager
{
    typedef void (*UpdateTimeThunk)(float, double, MonoObject**);
private:
    std::chrono::high_resolution_clock::time_point last_frame_time;
    float delta_time;
    double total_time;
    UpdateTimeThunk update_time_ptr = nullptr;
    MonoMethod* update_time_method = nullptr;
public:
    void Initialize(MonoImage* img) {
        last_frame_time = std::chrono::high_resolution_clock::now();


        MonoClass* time_class = mono_class_from_name(img, "Scripts.TimeTracking", "Time");
        if (!time_class) {
            std::cout << "Error: time class missing" << "\n";
            exit(0);
        }
        update_time_method = mono_class_get_method_from_name(time_class, "UpdateTime", 2);
        if (!update_time_method) {
            std::cout << "Error: couldn't find update time method in time class" << "\n";
            exit(0);
        }

        update_time_ptr = (UpdateTimeThunk)mono_method_get_unmanaged_thunk(update_time_method);
    }

    void Update() {
        auto curr_frame_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = curr_frame_time - last_frame_time;
        delta_time = elapsed.count();
        total_time += delta_time;
        last_frame_time = curr_frame_time;

        // maybe add something so debugging is a bit easier?
        if (delta_time > 0.1f) delta_time = 0.1f; // clamp just to be safe, might remove later

        MonoObject* ex = nullptr;
        update_time_ptr(delta_time, total_time, &ex);

        if (ex) {
            std::cout << "Error in TimeManager" << "\n";
            exit(0);
        }
    }

    float GetDeltaTime() const { return delta_time; }
    double GetTotalTime() const { return delta_time; }
};

