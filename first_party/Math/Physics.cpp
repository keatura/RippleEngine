#include "Math/Physics.h"

#include "Math/Rigidbody.h"
#include "Math/HitResult.h"
#include "Math/RaycastCallback.h"
#include "Math/MultiRaycastCallback.h"

#include <algorithm>

void Physics::Initialize() {
    b2_world = new b2World(b2Vec2(0.0f, 9.8f));
    b2_world->SetContactListener(&detector);

    //Vector2::Initialize(lua_state);
    Rigidbody::Initialize(b2_world);
    //Collision::Initialize(lua_state);
    //HitResult::Initialize(lua_state);

    /*luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Physics")
        .addFunction("Raycast", &Physics::Raycast)
        .addFunction("RaycastAll", &Physics::RaycastAll)
        .endNamespace();*/
}

void Physics::PhysicsStep(float time_delta) {
    b2_world->Step(time_delta, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

void Physics::Raycast(b2Vec2 pos, b2Vec2 dir, float dist)
{
    RaycastCallback callback;
    b2_world->RayCast(&callback, pos, { pos.x + (dir.x * dist), pos.y + (dir.y * dist) });
    float fraction = callback.m_fraction;

    if (fraction == 0.0f || fraction == 1.0f || fraction == -1.0f || callback.m_fixture == nullptr) {
        return; // TODO
    }

    HitResult result{
        callback.m_point,
        callback.m_normal,
        (callback.m_fixture->IsSensor() && callback.m_fixture->GetFilterData().categoryBits != 0xBEEF),
    };

    return; // TODO
}

void Physics::RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist)
{
    MultiRaycastCallback callback;
    b2_world->RayCast(&callback, pos, { pos.x + (dir.x * dist), pos.y + (dir.y * dist) });
    std::sort(callback.hits.begin(), callback.hits.end());
    size_t curr_idx = 1;
    for (MultiRaycastCallback::HitInfo& hit : callback.hits) {
        if (hit.fraction == 0 || hit.fraction == 1 || hit.fraction == -1.0f || hit.fixture == nullptr) {
            continue;
        }
        HitResult result = {
            hit.point,
            hit.normal,
            (hit.fixture->IsSensor() && hit.fixture->GetFilterData().categoryBits != 0xBEEF),
        };
        curr_idx++;
    }
    return; // TODO
}

