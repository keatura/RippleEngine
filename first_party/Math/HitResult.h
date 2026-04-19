#pragma once

#ifndef HITRESULT_H
#define HITRESULT_H

#include "box2d/box2d.h"
#include "box2d/b2_math.h"

struct HitResult {
    b2Vec2 point = { -999.0f, -999.0f };
    b2Vec2 normal = { -999.0f, -999.0f };
    bool is_trigger = false;
};
#endif
