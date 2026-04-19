#include "Math/Rigidbody.h"
#include "glm/glm.hpp"

#include <string>

Rigidbody::Rigidbody(Rigidbody& other)
{
    this->position.x = other.position.x;
    this->position.y = other.position.y;
    this->body_type = std::string(other.body_type);
    this->precise = other.precise;
    this->gravity_scale = other.gravity_scale;
    this->density = other.density;
    this->angular_friction = other.angular_friction;
    this->rotation = other.rotation;
    this->has_collider = other.has_collider;
    this->has_trigger = other.has_trigger;

    this->trigger_type = std::string(other.trigger_type);
    this->trigger_height = other.trigger_height;
    this->trigger_width = other.trigger_width;
    this->trigger_radius = other.trigger_radius;

    this->width = other.width;
    this->height = other.height;
    this->radius = other.radius;
    this->friction = other.friction;
    this->bounciness = other.bounciness;
    this->collider_type = std::string(other.collider_type);
    
    this->key = std::string(other.key);
}

void Rigidbody::Initialize(b2World* world) {
    b2_world = world;
}

b2Vec2 Rigidbody::GetPosition()
{
    if (body) return body->GetPosition();
    else return position;
}

float Rigidbody::GetRotation()
{
    if (body) return RadToDeg(body->GetAngle());
    else return rotation;
}

float Rigidbody::GetX() const
{
    if (body) return body->GetPosition().x;
    return position.x;
}

float Rigidbody::GetY() const
{
    if (body) return body->GetPosition().y;
    return position.y;
}

void Rigidbody::SetX(float x) {
    if (body) body->SetTransform(b2Vec2(x, body->GetPosition().y), body->GetAngle());
    else position.x = x;
}

void Rigidbody::SetY(float y) {
    if (body) body->SetTransform(b2Vec2(body->GetPosition().x, y), body->GetAngle());
    else position.y = y;
}

void Rigidbody::OnStart() {
    CreateBody();
    if (has_collider) {
        CreateColliderFixture();
    }
    if (has_trigger) {
        CreateTriggerFixture();
    }

    if (!(has_collider || has_trigger)) {
        CreateDefaultFixture();
    }
}

void Rigidbody::CreateBody()
{
    b2BodyDef body_def;
    if (body_type == "dynamic") {
        body_def.type = b2_dynamicBody;
    }
    else if (body_type == "kinematic") {
        body_def.type = b2_kinematicBody;
    }
    else if (body_type == "static") {
        body_def.type = b2_staticBody;
    }

    body_def.gravityScale = gravity_scale;
    body_def.position = b2Vec2(position.x, position.y);
    body_def.bullet = precise;
    body_def.angle = DegToRad(rotation);
    body_def.angularDamping = angular_friction;

    body = b2_world->CreateBody(&body_def);
}
 
void Rigidbody::CreateColliderFixture()
{
    b2Shape* shape = nullptr;
    if (collider_type == "box") {
        b2PolygonShape* polygon_shape = new b2PolygonShape();
        polygon_shape->SetAsBox(width * 0.5f, height * 0.5f);
        shape = polygon_shape;
    }
    else if (collider_type == "circle") {
        b2CircleShape* circle_shape = new b2CircleShape();
        circle_shape->m_radius = radius;
        shape = circle_shape;
    }

    b2FixtureDef fixture_def;
    fixture_def.shape = shape;
    fixture_def.density = density;
    fixture_def.isSensor = false;
    fixture_def.restitution = bounciness;
    fixture_def.friction = friction;
    
    fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(this);

    body->CreateFixture(&fixture_def);
}

void Rigidbody::CreateTriggerFixture()
{
    b2Shape* shape = nullptr;
    if (trigger_type == "box") {
        b2PolygonShape* polygon_shape = new b2PolygonShape();
        polygon_shape->SetAsBox(trigger_width * 0.5f, trigger_height * 0.5f);
        shape = polygon_shape;
    }
    else if (trigger_type == "circle") {
        b2CircleShape* circle_shape = new b2CircleShape();
        circle_shape->m_radius = trigger_radius;
        shape = circle_shape;
    }

    b2FixtureDef fixture_def;
    fixture_def.shape = shape;
    fixture_def.density = density;
    fixture_def.isSensor = true;

    fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(this);

    body->CreateFixture(&fixture_def);
}

inline constexpr uint16_t PHANTOM_BITS = 0xBEEF;
void Rigidbody::CreateDefaultFixture()
{
    b2PolygonShape phantom_shape;
    phantom_shape.SetAsBox(width * 0.5f, height * 0.5f);

    b2FixtureDef phantom_fixture_def;
    phantom_fixture_def.shape = &phantom_shape;
    phantom_fixture_def.density = density;

    phantom_fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(this);

    phantom_fixture_def.isSensor = true;
    phantom_fixture_def.filter.categoryBits = PHANTOM_BITS;
    body->CreateFixture(&phantom_fixture_def);
}

void Rigidbody::AddForce(b2Vec2 force)
{
    body->ApplyForceToCenter(force, true);
}

void Rigidbody::SetVelocity(b2Vec2 new_veloc)
{
    body->SetLinearVelocity(new_veloc);
}

void Rigidbody::SetPosition(b2Vec2 new_position)
{
    if (body) body->SetTransform(new_position, body->GetAngle());
    else position = new_position;
}

void Rigidbody::SetRotation(float degrees_clockwise)
{
    body->SetTransform(body->GetPosition(), DegToRad(degrees_clockwise));
}

void Rigidbody::SetAngularVelocity(float degrees_clockwise)
{
    body->SetAngularVelocity(DegToRad(degrees_clockwise));
}

void Rigidbody::SetGravityScale(float grav_scale)
{
    body->SetGravityScale(grav_scale);
}

void Rigidbody::SetUpDirection(b2Vec2 new_up_direction)
{
    new_up_direction.Normalize();
    float radians = glm::atan(new_up_direction.x, -new_up_direction.y);
    body->SetTransform(body->GetPosition(), radians);
}

void Rigidbody::SetRightDirection(b2Vec2 new_right_direction)
{
    new_right_direction.Normalize();
    float radians = glm::atan(new_right_direction.x, -new_right_direction.y) - RIGHT_DEGREES_CONSTANT;
    body->SetTransform(body->GetPosition(), radians);
}

b2Vec2 Rigidbody::GetVelocity()
{
    return body->GetLinearVelocity();
}

float Rigidbody::GetAngularVelocity()
{
    return RadToDeg(body->GetAngularVelocity());
}

float Rigidbody::GetGravityScale()
{
    return body->GetGravityScale();
}

b2Vec2 Rigidbody::GetUpDirection()
{
    float curr_radians = body->GetAngle() - RIGHT_DEGREES_CONSTANT;
    b2Vec2 result = b2Vec2(glm::cos(curr_radians), glm::sin(curr_radians));
    result.Normalize();
    return result;
}

b2Vec2 Rigidbody::GetRightDirection()
{ 
    float curr_radians = body->GetAngle();
    b2Vec2 result = b2Vec2(glm::cos(curr_radians), glm::sin(curr_radians));
    result.Normalize();
    return result;
}

#define DllExport __declspec(dllexport)

extern "C" {
    DllExport void* CreateBody(
        const char* body_type,
        const char* collider_type,
        const char* trigger_type,
        float grav_scale,
        float density,
        float angular_friction,
        float rotation,
        float width,
        float height,
        float radius,
        float friction,
        float bounciness,
        float trigger_width,
        float trigger_height,
        float trigger_radius,
        float x,
        float y,
        bool precise,
        bool has_collider,
        bool has_trigger
    ) {
        Rigidbody::rb_storage.push_back(std::make_unique<Rigidbody>());
        Rigidbody* rb = Rigidbody::rb_storage.back().get();

        rb->body_type = std::string(body_type);
        rb->collider_type = std::string(collider_type);
        rb->trigger_type = std::string(trigger_type);
        rb->gravity_scale = grav_scale;
        rb->density = density;
        rb->angular_friction = angular_friction;
        rb->rotation = rotation;
        rb->width = width;
        rb->height = height;
        rb->radius = radius;
        rb->friction = friction;
        rb->bounciness = bounciness;
        rb->trigger_width = trigger_width;
        rb->trigger_height = trigger_height;
        rb->trigger_radius = trigger_radius;
        rb->SetPosition(b2Vec2{ x, y });
        rb->precise = precise;
        rb->has_collider = has_collider;
        rb->has_trigger = has_trigger;
        rb->OnStart();

        return rb;
    }

    DllExport float GetX(void* body_ptr) {
        if (!body_ptr) return BASE_X_POS;
        return static_cast<Rigidbody*>(body_ptr)->GetPosition().x;
    }
    DllExport float GetY(void* body_ptr) {
        if (!body_ptr) return BASE_Y_POS;
        return static_cast<Rigidbody*>(body_ptr)->GetPosition().y;
    }

    DllExport void SetX(void* body_ptr, float x) {
        if (!body_ptr) return;
        Rigidbody* rb = static_cast<Rigidbody*>(body_ptr);
        float y = rb->GetPosition().y;
        rb->SetPosition(b2Vec2{ x, y });
    }

    DllExport void SetY(void* body_ptr, float y) {
        if (!body_ptr) return;
        Rigidbody* rb = static_cast<Rigidbody*>(body_ptr);
        float x = rb->GetPosition().x;
        rb->SetPosition(b2Vec2{ x, y });
    }

    DllExport void AddForce(void* body_ptr, float force_x, float force_y) {
        if (!body_ptr) return;
        static_cast<Rigidbody*>(body_ptr)->AddForce(b2Vec2{ force_x, force_y });
    }

    DllExport void SetVelocity(void* body_ptr, float vel_x, float vel_y) {
        if (!body_ptr) return;
        static_cast<Rigidbody*>(body_ptr)->SetVelocity(b2Vec2{ vel_x, vel_y });
    }


    DllExport void SetPosition(void* body_ptr, float pos_x, float pos_y) {
        if (!body_ptr) return;
        static_cast<Rigidbody*>(body_ptr)->SetPosition(b2Vec2{ pos_x, pos_y });
    }

    DllExport void SetRotation(void* body_ptr, float degrees_clockwise) {
        if (!body_ptr) return;
        static_cast<Rigidbody*>(body_ptr)->SetRotation(degrees_clockwise);
    }

    DllExport void SetAngularVelocity(void* body_ptr, float degrees_clockwise) {
        if (!body_ptr) return;
        static_cast<Rigidbody*>(body_ptr)->SetAngularVelocity(degrees_clockwise);
    }

    DllExport void SetGravityScale(void* body_ptr, float gravity_scale) {
        if (!body_ptr) return;
        static_cast<Rigidbody*>(body_ptr)->SetGravityScale(gravity_scale);
    }
    
    DllExport void SetUpDirection(void* body_ptr, float up_direction_x, float up_direction_y) {
        if (!body_ptr) return;
        static_cast<Rigidbody*>(body_ptr)->SetUpDirection(b2Vec2{ up_direction_x, up_direction_y });
    }

    DllExport void SetRightDirection(void* body_ptr, float right_direction_x, float right_direction_y) {
        if (!body_ptr) return;
        static_cast<Rigidbody*>(body_ptr)->SetRightDirection(b2Vec2{ right_direction_x, right_direction_y });
    }

    DllExport float GetAngularVelocity(void* body_ptr) {
        if (!body_ptr) return 0.0f;
        return static_cast<Rigidbody*>(body_ptr)->GetAngularVelocity();
    }

    DllExport float GetGravityScale(void* body_ptr) {
        if (!body_ptr) return BASE_GRAV_SCALE;
        return static_cast<Rigidbody*>(body_ptr)->GetGravityScale();
    }

    DllExport float GetRotation(void* body_ptr) {
        if (!body_ptr) return BASE_ROTATION;
        return static_cast<Rigidbody*>(body_ptr)->GetRotation();
    }

    // vector stuff
    DllExport void GetVelocity_Inner(void* body_ptr, float* vel_x, float* vel_y) {
        if (!body_ptr) {
            *vel_x = 0.0f;
            *vel_y = 0.0f;
            return;
        }
        b2Vec2 vel = static_cast<Rigidbody*>(body_ptr)->GetVelocity();
        *vel_x = vel.x;
        *vel_y = vel.y;
    }

    DllExport void GetUpDirection_Inner(void* body_ptr, float* up_x, float* up_y) {
        if (!body_ptr) {
            *up_x = 0.0f;
            *up_y = 1.0f;
            return;
        }
        b2Vec2 up = static_cast<Rigidbody*>(body_ptr)->GetUpDirection();
        *up_x = up.x;
        *up_y = up.y;
    }

    DllExport void GetRightDirection_Inner(void* body_ptr, float* right_x, float* right_y) {
        if (!body_ptr) {
            *right_x = 1.0f;
            *right_y = 0.0f;
            return;
        }
        b2Vec2 right = static_cast<Rigidbody*>(body_ptr)->GetRightDirection();
        *right_x = right.x;
        *right_y = right.y;
    }

    DllExport void GetPosition_Inner(void* body_ptr, float* position_x, float* position_y) {
        if (!body_ptr) {
            *position_x = BASE_X_POS;
            *position_y = BASE_Y_POS;
            return;
        }
        b2Vec2 position = static_cast<Rigidbody*>(body_ptr)->GetPosition();
        *position_x = position.x;
        *position_y = position.y;
    }

    DllExport void DestroyBody(void* body_ptr) {
        if (!body_ptr) return;

        Rigidbody* target_rb = static_cast<Rigidbody*>(body_ptr);

        if (target_rb->body && target_rb->body->GetWorld()) {
            target_rb->body->GetWorld()->DestroyBody(target_rb->body);
            target_rb->body = nullptr;
        }

        auto& storage = Rigidbody::rb_storage;
        for (size_t i = 0; i < storage.size(); i++) {
            if (storage[i].get() == target_rb) {
                if (i != storage.size() - 1) {
                    std::swap(storage[i], storage.back());
                }

                storage.pop_back();
                break;
            }
        }
    }
}