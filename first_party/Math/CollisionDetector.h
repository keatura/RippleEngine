#pragma once

#ifndef COLLISIONMANAGEMENT_H
#define COLLISIONMANAGEMENT_H

#include "box2d/box2d.h"
#include "box2d/b2_math.h"

class CollisionDetector : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
};

#endif
