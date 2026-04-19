#include "ParticleSystem.h"
#include "Graphics/Renderer.h"
#include "DB/ImageDB.h"
#include "Utils/Application.h"

#include <algorithm>

void ParticleSystem::Initialize() {
    /*lua_state = ls;
    luabridge::getGlobalNamespace(ls)
        .beginClass<ParticleSystem>("ParticleSystem")
        .addData("actor", &ParticleSystem::actor)
        .addData("key", &ParticleSystem::key)
        .addData("type", &ParticleSystem::type)

        .addData("image", &ParticleSystem::image)

        .addData("frames_between_bursts", &ParticleSystem::frames_between_bursts)
        .addData("burst_quantity", &ParticleSystem::burst_quantity)
        .addData("duration_frames", &ParticleSystem::duration_frames)

        .addData("sorting_order", &ParticleSystem::sorting_order)

        .addData("x", &ParticleSystem::x)
        .addData("y", &ParticleSystem::y)

        .addData("start_scale_min", &ParticleSystem::start_scale_min)
        .addData("start_scale_max", &ParticleSystem::start_scale_max)

        .addData("rotation_min", &ParticleSystem::rotation_min)
        .addData("rotation_max", &ParticleSystem::rotation_max)

        .addData("emit_radius_min", &ParticleSystem::emit_radius_min)
        .addData("emit_radius_max", &ParticleSystem::emit_radius_max)

        .addData("emit_angle_min", &ParticleSystem::emit_angle_min)
        .addData("emit_angle_max", &ParticleSystem::emit_angle_max)

        .addData("start_speed_min", &ParticleSystem::start_speed_min)
        .addData("start_speed_max", &ParticleSystem::start_speed_max)

        .addData("rotation_speed_min", &ParticleSystem::rotation_speed_min)
        .addData("rotation_speed_max", &ParticleSystem::rotation_speed_max)

        .addData("gravity_scale_x", &ParticleSystem::gravity_scale_x)
        .addData("gravity_scale_y", &ParticleSystem::gravity_scale_y)

        .addData("drag_factor", &ParticleSystem::drag_factor)
        .addData("angular_drag_factor", &ParticleSystem::angular_drag_factor)

        .addData("end_scale", &ParticleSystem::end_scale)

        .addData("start_color_r", &ParticleSystem::start_color_r)
        .addData("start_color_g", &ParticleSystem::start_color_g)
        .addData("start_color_b", &ParticleSystem::start_color_b)
        .addData("start_color_a", &ParticleSystem::start_color_a)
        .addData("end_color_r", &ParticleSystem::end_color_r)
        .addData("end_color_g", &ParticleSystem::end_color_g)
        .addData("end_color_b", &ParticleSystem::end_color_b)
        .addData("end_color_a", &ParticleSystem::end_color_a)

        .addData("enabled", &ParticleSystem::enabled)

        .addFunction("OnStart", &ParticleSystem::OnStart)
        .addFunction("OnUpdate", &ParticleSystem::OnUpdate)
        .addFunction("Play", &ParticleSystem::Play)
        .addFunction("Stop", &ParticleSystem::Stop)
        .addFunction("Burst", &ParticleSystem::Burst)
        .endClass();*/
}

ParticleSystem::ParticleSystem(ParticleSystem& other)
{
    this->key = std::string(other.key);

    this->image = std::string(other.image);
    this->frames_between_bursts = other.frames_between_bursts;
    this->burst_quantity = other.burst_quantity;
    this->duration_frames = other.duration_frames;
    this->sorting_order = other.sorting_order;
    this->x = other.x;
    this->y = other.y;
    this->start_scale_min = other.start_scale_min;
    this->start_scale_max = other.start_scale_max;
    this->rotation_min = other.rotation_min;
    this->rotation_max = other.rotation_max;
    this->emit_radius_min = other.emit_radius_min;
    this->emit_radius_max = other.emit_radius_max;
    this->emit_angle_min = other.emit_angle_min;
    this->emit_angle_max = other.emit_angle_max;
    this->start_speed_min = other.start_speed_min;
    this->start_speed_max = other.start_speed_max;
    this->rotation_speed_min = other.rotation_speed_min;
    this->rotation_speed_max = other.rotation_speed_max;
    this->gravity_scale_x = other.gravity_scale_x;
    this->gravity_scale_y = other.gravity_scale_y;
    this->drag_factor = other.drag_factor;
    this->angular_drag_factor = other.angular_drag_factor;
    this->end_scale = other.end_scale;
    this->start_color_r = other.start_color_r;
    this->start_color_g = other.start_color_g;
    this->start_color_b = other.start_color_b;
    this->start_color_a = other.start_color_a;
    this->end_color_r = other.end_color_r;
    this->end_color_g = other.end_color_g;
    this->end_color_b = other.end_color_b;
    this->end_color_a = other.end_color_a;

    this->enabled = true;

    start_frame_num = Application::GetFrame();
    // copy constructor
}

void ParticleSystem::MakeParticleSystemComponent(std::string comp_key, const rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>>& json_val)
{
    /*ps_storage.push_back(std::make_unique<ParticleSystem>());
    ParticleSystem* ps = ps_storage.back().get();

    ps->key = comp_key;
    ps->image = (json_val.HasMember("image")) ? json_val["image"].GetString() : DEFAULT_IMAGE;

    ps->frames_between_bursts = (json_val.HasMember("frames_between_bursts")) ? json_val["frames_between_bursts"].GetUint() : DEFAULT_FRAMES_BETWEEN_BURSTS;
    ps->burst_quantity = (json_val.HasMember("burst_quantity")) ? json_val["burst_quantity"].GetInt() : DEFAULT_BURST_QUANTITY;
    ps->duration_frames = (json_val.HasMember("duration_frames")) ? ((json_val["duration_frames"].GetInt() < 1) ? 1 : json_val["duration_frames"].GetInt()) : DEFAULT_DURATION_FRAMES;
    ps->sorting_order = (json_val.HasMember("sorting_order")) ? json_val["sorting_order"].GetInt() : DEFAULT_SORTING_ORDER;

    ps->x = (json_val.HasMember("x")) ? json_val["x"].GetFloat() : DEFAULT_ORIGIN_X;
    ps->y = (json_val.HasMember("y")) ? json_val["y"].GetFloat() : DEFAULT_ORIGIN_Y;
    ps->start_scale_min = (json_val.HasMember("start_scale_min")) ? json_val["start_scale_min"].GetFloat() : DEFAULT_START_SCALE_MIN;
    ps->start_scale_max = (json_val.HasMember("start_scale_max")) ? json_val["start_scale_max"].GetFloat() : DEFAULT_START_SCALE_MAX;
    ps->rotation_min = (json_val.HasMember("rotation_min")) ? json_val["rotation_min"].GetFloat() : DEFAULT_ROTATION_MIN;
    ps->rotation_max = (json_val.HasMember("rotation_max")) ? json_val["rotation_max"].GetFloat() : DEFAULT_ROTATION_MAX;
    ps->emit_radius_min = (json_val.HasMember("emit_radius_min")) ? json_val["emit_radius_min"].GetFloat() : DEFAULT_EMIT_RADIUS_MIN;
    ps->emit_radius_max = (json_val.HasMember("emit_radius_max")) ? json_val["emit_radius_max"].GetFloat() : DEFAULT_EMIT_RADIUS_MAX;
    ps->emit_angle_min = (json_val.HasMember("emit_angle_min")) ? json_val["emit_angle_min"].GetFloat() : DEFAULT_EMIT_ANGLE_MIN;
    ps->emit_angle_max = (json_val.HasMember("emit_angle_max")) ? json_val["emit_angle_max"].GetFloat() : DEFAULT_EMTI_ANGLE_MAX;
    ps->start_speed_min = (json_val.HasMember("start_speed_min")) ? json_val["start_speed_min"].GetFloat() : DEFAULT_START_SPEED_MIN;
    ps->start_speed_max = (json_val.HasMember("start_speed_max")) ? json_val["start_speed_max"].GetFloat() : DEFAULT_START_SPEED_MAX;
    ps->rotation_speed_min = (json_val.HasMember("rotation_speed_min")) ? json_val["rotation_speed_min"].GetFloat() : DEFAULT_ROTATION_SPEED_MIN;
    ps->rotation_speed_max = (json_val.HasMember("rotation_speed_max")) ? json_val["rotation_speed_max"].GetFloat() : DEFAULT_ROTATION_SPEED_MAX;
    ps->gravity_scale_x = (json_val.HasMember("gravity_scale_x")) ? json_val["gravity_scale_x"].GetFloat() : DEFAULT_GRAVITY_SCALE_X;
    ps->gravity_scale_y = (json_val.HasMember("gravity_scale_y")) ? json_val["gravity_scale_y"].GetFloat() : DEFAULT_GRAVITY_SCALE_Y;
    ps->drag_factor = (json_val.HasMember("drag_factor")) ? json_val["drag_factor"].GetFloat() : DEFAULT_DRAG_FACTOR;
    ps->angular_drag_factor = (json_val.HasMember("angular_drag_factor")) ? json_val["angular_drag_factor"].GetFloat() : DEFAULT_ANGULAR_DRAG_FACTOR;
    ps->end_scale = (json_val.HasMember("end_scale")) ? json_val["end_scale"].GetFloat() : -1;
    ps->start_color_r = (json_val.HasMember("start_color_r")) ? json_val["start_color_r"].GetInt() : DEFAULT_START_COLOR_R;
    ps->start_color_g = (json_val.HasMember("start_color_g")) ? json_val["start_color_g"].GetInt() : DEFAULT_START_COLOR_G;
    ps->start_color_b = (json_val.HasMember("start_color_b")) ? json_val["start_color_b"].GetInt() : DEFAULT_START_COLOR_B;
    ps->start_color_a = (json_val.HasMember("start_color_a")) ? json_val["start_color_a"].GetInt() : DEFAULT_START_COLOR_A;
    ps->end_color_r = (json_val.HasMember("end_color_r")) ? json_val["end_color_r"].GetInt() : ps->start_color_r;
    ps->end_color_g = (json_val.HasMember("end_color_g")) ? json_val["end_color_g"].GetInt() : ps->start_color_g;
    ps->end_color_b = (json_val.HasMember("end_color_b")) ? json_val["end_color_b"].GetInt() : ps->start_color_b;
    ps->end_color_a = (json_val.HasMember("end_color_a")) ? json_val["end_color_a"].GetInt() : ps->start_color_a;

    ps->enabled = (json_val.HasMember("enabled")) ? json_val["enabled"].GetBool() : true;*/
}

void ParticleSystem::MakeEmptyParticleSystem(std::string comp_key)
{
    ps_storage.push_back(std::make_unique<ParticleSystem>());
    ParticleSystem* ps = ps_storage.back().get();

    ps->key = comp_key;
}

void ParticleSystem::OnStart() {
    //this->emit_angle_distribution = 0; // RandomEngine(emit_angle_min, emit_angle_max, 298);
    //this->emit_radius_distribution = 0; // RandomEngine(emit_radius_min, emit_radius_max, 404);

    //this->rotation_distribution = 0; // RandomEngine(rotation_min, rotation_max, 440);
    //this->scale_distribution = 0; // RandomEngine(start_scale_min, start_scale_max, 494);

    //this->speed_distribution = 0; // RandomEngine(start_speed_min, start_speed_max, 498);
    //this->rotation_speed_distribution = 0; // RandomEngine(rotation_speed_min, rotation_speed_max, 305);

    this->local_frame_num = Application::GetFrame();
}

void ParticleSystem::CopyParticleSystem(int other_ps_ref)
{
    /*ps_storage.push_back(std::make_unique<ParticleSystem>());
    ParticleSystem* ps = ps_storage.back().get();

    ParticleSystem* other = other_ps_ref.cast<ParticleSystem*>();

    ps->key = std::string(other->key);
    ps->type = std::string(other->type);
    ps->image = std::string(other->image);
    ps->frames_between_bursts = other->frames_between_bursts;
    ps->burst_quantity = other->burst_quantity;
    ps->duration_frames = other->duration_frames;
    ps->sorting_order = other->sorting_order;
    ps->x = other->x;
    ps->y = other->y;
    ps->start_scale_min = other->start_scale_min;
    ps->start_scale_max = other->start_scale_max;
    ps->rotation_min = other->rotation_min;
    ps->rotation_max = other->rotation_max;
    ps->emit_radius_min = other->emit_radius_min;
    ps->emit_radius_max = other->emit_radius_max;
    ps->emit_angle_min = other->emit_angle_min;
    ps->emit_angle_max = other->emit_angle_max;
    ps->start_speed_min = other->start_speed_min;
    ps->start_speed_max = other->start_speed_max;
    ps->rotation_speed_min = other->rotation_speed_min;
    ps->rotation_speed_max = other->rotation_speed_max;
    ps->gravity_scale_x = other->gravity_scale_x;
    ps->gravity_scale_y = other->gravity_scale_y;
    ps->drag_factor = other->drag_factor;
    ps->angular_drag_factor = other->angular_drag_factor;
    ps->end_scale = other->end_scale;
    ps->start_color_r = other->start_color_r;
    ps->start_color_g = other->start_color_g;
    ps->start_color_b = other->start_color_b;
    ps->start_color_a = other->start_color_a;
    ps->end_color_r = other->end_color_r;
    ps->end_color_g = other->end_color_g;
    ps->end_color_b = other->end_color_b;
    ps->end_color_a = other->end_color_a;

    ps->enabled = true;

    luabridge::LuaRef ps_ref = luabridge::LuaRef(lua_state, ps);
    return ps_ref;*/
}

// HERE

inline constexpr float HARDCODED_XY_PIVOT = 0.5f;
void ParticleSystem::OnUpdate() {
    float
        angle_radians = 0,
        radius = 0,
        cos_angle = 0,
        sin_angle = 0,
        starting_x_pos = 0,
        starting_y_pos = 0;

    RefreshClampedData();

    if (!paused && (local_frame_num % frames_between_bursts) == 0) {
        CreateNewParticles();
    }

    ApplyParticleTransformations();


    // scale stuff
    std::vector<float> lifetime_progress_lookup_table(duration_frames, 0.0f);
    for (size_t i = 0; i < duration_frames; i++) {
        lifetime_progress_lookup_table[i] = static_cast<float>(i) / duration_frames;
    }

    float lifetime_progress = 0;
    if (end_scale != -1) {
        for (size_t idx : active_indices) {
            lifetime_progress = lifetime_progress_lookup_table[local_frame_num - spawn_frames[idx]];
            curr_scales[idx] = glm::mix(start_scales[idx], end_scale, lifetime_progress);
        }
    }

    std::vector<SDL_Color> color_lookup_table(duration_frames);
    for (size_t i = 0; i < duration_frames; i++) {
        lifetime_progress = lifetime_progress_lookup_table[i];
        color_lookup_table[i] = {
            glm::mix(start_color_r, end_color_r, lifetime_progress),
            glm::mix(start_color_g, end_color_g, lifetime_progress),
            glm::mix(start_color_b, end_color_b, lifetime_progress),
            glm::mix(start_color_a, end_color_a, lifetime_progress)
        };
    }

    int image_idx = ImageDB::GetTextureIdx(image);

    // culling stuff
    fvec2 dims = { 0,0 }; // ImageDB::GetDimensions(image_idx);

    float diagonal = std::sqrt((dims.x * dims.x) + (dims.y * dims.y));
    float base_radius = diagonal/2.0f;

    float visible_width = (float)Renderer::x_resolution / Renderer::zoom_factor;
    float visible_height = (float)Renderer::y_resolution / Renderer::zoom_factor;

    float cam_left = Renderer::curr_cam_position.x - (visible_width / 2.0f);
    float cam_right = Renderer::curr_cam_position.x + (visible_width / 2.0f);
    float cam_top = Renderer::curr_cam_position.y - (visible_height / 2.0f);
    float cam_bottom = Renderer::curr_cam_position.y + (visible_height / 2.0f);


    for (size_t idx : active_indices) {
        fvec2 pos = positions[idx];
        float scale = curr_scales[idx];

        float particle_radius = base_radius * scale;

        if (pos.x + particle_radius >= cam_left && pos.x - particle_radius <= cam_right
            && pos.y + particle_radius >= cam_top && pos.y - particle_radius <= cam_bottom)
        {
            float rotation = rotations[idx];
            SDL_Color color = color_lookup_table[local_frame_num - spawn_frames[idx]];

            Renderer::Image_DrawEx_Fast(image_idx, pos.x, pos.y, rotation, scale, scale, HARDCODED_XY_PIVOT, HARDCODED_XY_PIVOT, color.r, color.g, color.b, color.a, sorting_order);
        }
    }

    local_frame_num++;
    RemoveDeadParticles();
}

void ParticleSystem::RefreshClampedData()
{
    if (this->duration_frames < 1) {
        this->duration_frames = 1;
    }

    if (this->frames_between_bursts < 1) {
        this->frames_between_bursts = 1;
    }

    if (this->burst_quantity < 1) {
        this->burst_quantity = 1;
    }

    size_t max_concurrent_bursts = (duration_frames / frames_between_bursts) + 1;
    size_t absolute_max_particles = max_concurrent_bursts * burst_quantity;

    if (curr_storage_size < absolute_max_particles) {
        size_t old_size = curr_storage_size;
        curr_storage_size = absolute_max_particles;
        spawn_frames.resize(curr_storage_size, 0);
        positions.resize(curr_storage_size, { 0.0f, 0.0f });
        velocities.resize(curr_storage_size, { 0.0f, 0.0f });
        rotations.resize(curr_storage_size, 0.0f);
        angular_velocities.resize(curr_storage_size, 0.0f);
        start_scales.resize(curr_storage_size, 0.0f);
        curr_scales.resize(curr_storage_size, 0.0f);

        for (size_t i = old_size; i < curr_storage_size; i++) {
            free_indices.push_back(i);
        }
    }
}

void ParticleSystem::RemoveDeadParticles() {
    uint16_t curr_frame = (uint16_t)local_frame_num;
    active_indices.erase(std::remove_if(active_indices.begin(), active_indices.end(),
        [&](size_t idx) {
            if ((uint16_t)(curr_frame - spawn_frames[idx]) >= duration_frames) {
                free_indices.push_front(idx);
                return true;
            }
            return false;
        }), 
        active_indices.end());
}

void ParticleSystem::CreateNewParticles() 
{
    // allocate indices to use for new particles
    std::vector<size_t> allocated_indices(burst_quantity, 0);
    size_t curr_idx = 0;
    while (curr_idx < burst_quantity && !free_indices.empty()) {
        allocated_indices[curr_idx] = free_indices.front();
        active_indices.push_back(free_indices.front());
        free_indices.pop_front();
        curr_idx++;
    }
    std::sort(active_indices.begin(), active_indices.end());
    // create new particles

    // emission shape and velocity
    float
        angle_radians = 0,
        radius = 0,
        cos_angle = 0,
        sin_angle = 0,
        starting_x_pos = 0,
        starting_y_pos = 0,
        speed = 0;
    for (size_t particle_idx : allocated_indices) {
        angle_radians = 0; // glm::radians(emit_angle_distribution.Sample());
        radius = 0; // emit_radius_distribution.Sample();
        cos_angle = glm::cos(angle_radians);
        sin_angle = glm::sin(angle_radians);
        starting_x_pos = x + cos_angle * radius;
        starting_y_pos = y + sin_angle * radius;
        positions[particle_idx] = { starting_x_pos, starting_y_pos };

        speed = 0; // speed_distribution.Sample();

        velocities[particle_idx] = { cos_angle * speed, sin_angle * speed };
    }

    float rotation_speed = 0;
    for (size_t particle_idx : allocated_indices) {
        rotation_speed = 0; // rotation_speed_distribution.Sample();
        angular_velocities[particle_idx] = rotation_speed;
    }

    // spawn frames
    for (size_t particle_idx : allocated_indices) {
        spawn_frames[particle_idx] = local_frame_num;
    }

    // start scale 
    float scale = 0;
    for (size_t particle_idx : allocated_indices) {
        scale = 0; // scale_distribution.Sample();
        start_scales[particle_idx] = scale;
        curr_scales[particle_idx] = scale;
    }

    // rotation
    float rotation = 0;
    for (size_t particle_idx : allocated_indices) {
        rotation = 0; // rotation_distribution.Sample();
        rotations[particle_idx] = rotation;
    }
}

void ParticleSystem::ApplyParticleTransformations() {
    fvec2 grav_scale = { gravity_scale_x, gravity_scale_y };
    
    for (size_t idx : active_indices) {
        velocities[idx] += grav_scale;
        velocities[idx] *= drag_factor;

        positions[idx] += velocities[idx];
    }

    for (size_t idx : active_indices) {
        angular_velocities[idx] *= angular_drag_factor;

        rotations[idx] += angular_velocities[idx];
    }
}

void ParticleSystem::Play() {
    paused = false;
}

void ParticleSystem::Stop() {
    paused = true;
}

void ParticleSystem::Burst() {
    if (active_indices.size() + burst_quantity > curr_storage_size) {
        size_t old_size = curr_storage_size;
        curr_storage_size = curr_storage_size*2;
        spawn_frames.resize(curr_storage_size, 0);
        positions.resize(curr_storage_size, { 0.0f, 0.0f });
        velocities.resize(curr_storage_size, { 0.0f, 0.0f });
        rotations.resize(curr_storage_size, 0.0f);
        angular_velocities.resize(curr_storage_size, 0.0f);
        start_scales.resize(curr_storage_size, 0.0f);
        curr_scales.resize(curr_storage_size, 0.0f);

        for (size_t i = old_size; i < curr_storage_size; i++) {
            free_indices.push_back(i);
        }
    }

    RefreshClampedData();
    CreateNewParticles();
}