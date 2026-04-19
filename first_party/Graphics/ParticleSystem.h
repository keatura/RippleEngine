#pragma once

#include "Graphics/ParticleSystemConstants.h"
#include "Math/FastVector.h"

#include <deque>
#include <memory>
#include <vector>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

struct SDL_Color;

class ParticleSystem
{
public:
    static void Initialize();
    ParticleSystem() {}
    ParticleSystem(ParticleSystem& other);

    static void MakeParticleSystemComponent(std::string comp_key, const rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>>& json_val);
    static void MakeEmptyParticleSystem(std::string comp_key);
    static void CopyParticleSystem(int other_ps_ref);

    void OnStart();
    void OnUpdate();
private:
    // particle manager stuff
    size_t curr_storage_size = 1;
    size_t num_active_particles = 0;
    std::vector<size_t> active_indices;
    std::deque<size_t> free_indices;

    std::vector<uint16_t> spawn_frames;
    std::vector<fvec2> positions;
    std::vector<fvec2> velocities;
    std::vector<float> rotations;
    std::vector<float> angular_velocities;
    std::vector<SDL_Color> curr_colors;

    std::vector<float> start_scales;
    std::vector<float> curr_scales;

    void RefreshClampedData();
    void RemoveDeadParticles();
    void CreateNewParticles();
    void ApplyParticleTransformations();
    void Play();
    void Burst();
    void Stop();



    static inline std::vector<std::unique_ptr<ParticleSystem>> ps_storage;

    std::string type = "ParticleSystem";
    std::string key = "";
    std::string image = DEFAULT_IMAGE;

    // RandomEngine emit_angle_distribution;
    // RandomEngine emit_radius_distribution;
    // RandomEngine rotation_distribution;
    // RandomEngine scale_distribution;
    // RandomEngine speed_distribution;
    // RandomEngine rotation_speed_distribution;

    size_t start_frame_num = 0;
    size_t local_frame_num = 0;
    size_t frames_between_bursts = DEFAULT_FRAMES_BETWEEN_BURSTS;
    size_t burst_quantity = DEFAULT_BURST_QUANTITY;
    size_t duration_frames = DEFAULT_DURATION_FRAMES;

    int32_t sorting_order = DEFAULT_SORTING_ORDER;

    float x = DEFAULT_ORIGIN_X;
    float y = DEFAULT_ORIGIN_Y;
    float start_scale_min = DEFAULT_START_SCALE_MIN;
    float start_scale_max = DEFAULT_START_SCALE_MAX;
    float rotation_min = DEFAULT_ROTATION_MIN;
    float rotation_max = DEFAULT_ROTATION_MAX;
    float emit_radius_min = DEFAULT_EMIT_RADIUS_MIN;
    float emit_radius_max = DEFAULT_EMIT_RADIUS_MAX;
    float emit_angle_min = DEFAULT_EMIT_ANGLE_MIN;
    float emit_angle_max = DEFAULT_EMTI_ANGLE_MAX;
    float start_speed_min = DEFAULT_START_SPEED_MIN;
    float start_speed_max = DEFAULT_START_SPEED_MAX;
    float rotation_speed_min = DEFAULT_ROTATION_SPEED_MIN;
    float rotation_speed_max = DEFAULT_ROTATION_SPEED_MAX; 
    float gravity_scale_x = DEFAULT_GRAVITY_SCALE_X;
    float gravity_scale_y = DEFAULT_GRAVITY_SCALE_Y;
    float drag_factor = DEFAULT_DRAG_FACTOR;
    float angular_drag_factor = DEFAULT_ANGULAR_DRAG_FACTOR; 
    float end_scale = DEFAULT_END_SCALE;

    uint8_t start_color_r = DEFAULT_START_COLOR_R;
    uint8_t start_color_g = DEFAULT_START_COLOR_G;
    uint8_t start_color_b = DEFAULT_START_COLOR_B; 
    uint8_t start_color_a = DEFAULT_START_COLOR_A;
    uint8_t end_color_r = DEFAULT_END_COLOR_R;
    uint8_t end_color_g = DEFAULT_END_COLOR_G;
    uint8_t end_color_b = DEFAULT_END_COLOR_B;
    uint8_t end_color_a = DEFAULT_END_COLOR_A;

    bool enabled = true;
    bool paused = false;
};