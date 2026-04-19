#pragma once

#ifndef COLLISION_H
#define COLLISION_H

struct lua_State;

#include "box2d/box2d.h"
#include "box2d/b2_math.h"

class Collision {
public:
    b2Vec2 point = { -999.0f, -999.0f };
    b2Vec2 relative_velocity = { -999.0f, -999.0f };
    b2Vec2 normal = { -999.0f, -999.0f };

    static void Initialize();
};
#endif