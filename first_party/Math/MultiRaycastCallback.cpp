#include "MultiRaycastCallback.h"

float MultiRaycastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
    if (fixture != nullptr && fixture->GetFilterData().categoryBits == 0xBEEF) {
        return 1.0f;
    }
    hits.push_back({ fixture, point, normal, fraction });
    return 1.0f;
}