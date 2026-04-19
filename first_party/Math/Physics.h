#pragma once

#ifndef PHYSICS_H
#define PHYSICS_H

#include "box2d/box2d.h"
#include "box2d/b2_math.h"

#include "Math/CollisionDetector.h"

class HitResult;

inline constexpr float TIME_STEP = 1.0f / 60.0f;
inline const int VELOCITY_ITERATIONS = 8;
inline const int POSITION_ITERATIONS = 3;

class Physics {
private:
    static inline b2World* b2_world;
    static inline CollisionDetector detector;
public:
    static void Initialize();
    static void PhysicsStep(float time_delta);
    static void Raycast(b2Vec2 pos, b2Vec2 dir, float dist); // TODO
    static void RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist); // TODO
};

#endif