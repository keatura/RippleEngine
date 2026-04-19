#pragma once

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Math/FastVector.h"
#include <deque>
#include <memory>
#include <vector>
#include <string>

#include "box2d/box2d.h"
#include "box2d/b2_math.h"

inline constexpr float BASE_X_POS = 0.0f;
inline constexpr float BASE_Y_POS = 0.0f;
inline constexpr bool BASE_PRECISION = true;
inline constexpr float BASE_GRAV_SCALE = 1.0f;
inline constexpr float BASE_DENSITY = 1.0f;
inline constexpr float BASE_ANGULAR_FRICTION = 0.3f;
inline constexpr float BASE_ROTATION = 0.0f;
inline constexpr bool BASE_HAS_COLLIDER = true;
inline constexpr bool BASE_HAS_TRIGGER = true;

inline constexpr float BASE_TRIGGER_WIDTH = 1.0f;
inline constexpr float BASE_TRIGGER_HEIGHT = 1.0f;
inline constexpr float BASE_TRIGGER_RADIUS = 0.5f;


inline constexpr float BASE_WIDTH = 1.0f;
inline constexpr float BASE_HEIGHT = 1.0f;
inline constexpr float BASE_RADIUS = 0.5f;
inline constexpr float BASE_FRICTION = 0.3f;
inline constexpr float BASE_BOUNCINESS = 0.3f;
inline constexpr float RIGHT_DEGREES_CONSTANT = (b2_pi / 2.0f);
inline constexpr float RADIANS_TO_DEGREES_CONSTANT = (180.0f / b2_pi);
inline constexpr float DEGREES_TO_RADIANS_CONSTANT = (b2_pi / 180.0f);

class Rigidbody
{
public:
    float GetX() const;
    float GetY() const;
    void SetX(float x);
    void SetY(float y);

    void OnStart();
    void CreateBody();
    void CreateColliderFixture();
    void CreateTriggerFixture();
    void CreateDefaultFixture();

    // rigidbody manipulation
    void AddForce(b2Vec2 force);
    void SetVelocity(b2Vec2 new_veloc);
    void SetPosition(b2Vec2 new_position);
    void SetRotation(float degrees_clockwise);
    void SetAngularVelocity(float degrees_clockwise);
    void SetGravityScale(float grav_scale);
    void SetUpDirection(b2Vec2 new_up_direction);
    void SetRightDirection(b2Vec2 new_right_direction);

    b2Vec2 GetVelocity();
    float GetAngularVelocity();
    float GetGravityScale();
    b2Vec2 GetUpDirection();
    b2Vec2 GetRightDirection();
    // end rigidbody manipulation

    Rigidbody() {}
    Rigidbody(Rigidbody& other);


    static void Initialize(b2World* world);

    std::string type = "Rigidbody";
    std::string key = "???";
    std::string body_type = "dynamic";
    std::string collider_type = "box";
    std::string trigger_type = "box";

    float gravity_scale = BASE_GRAV_SCALE;
    float density = BASE_DENSITY;
    float angular_friction = BASE_ANGULAR_FRICTION;
    float rotation = BASE_ROTATION;
    float width = BASE_WIDTH;
    float height = BASE_HEIGHT;
    float radius = BASE_RADIUS;
    float friction = BASE_FRICTION;
    float bounciness = BASE_BOUNCINESS;

    float trigger_width = BASE_TRIGGER_WIDTH;
    float trigger_height = BASE_TRIGGER_HEIGHT;
    float trigger_radius = BASE_TRIGGER_RADIUS;


    bool precise = BASE_PRECISION;
    bool has_collider = BASE_HAS_COLLIDER;
    bool has_trigger = BASE_HAS_TRIGGER;
    bool enabled = true;

    b2Vec2 GetPosition();
    float GetRotation();

    static inline std::vector<std::unique_ptr<Rigidbody>> rb_storage;
    b2Body* body = nullptr;
private:
    [[nodiscard]] inline constexpr float RadToDeg(float rad) const noexcept {
        return  rad * RADIANS_TO_DEGREES_CONSTANT;
    }
    [[nodiscard]] inline constexpr float DegToRad(float degree) const noexcept {
        return  degree * DEGREES_TO_RADIANS_CONSTANT;
    }
    b2Vec2 position = { 0.0f,0.0f };

    static inline b2World* b2_world;
};

#endif
