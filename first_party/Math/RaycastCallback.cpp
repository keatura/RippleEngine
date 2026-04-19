#include "Math/RaycastCallback.h"

float RaycastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
    if (fixture != nullptr && fixture->GetFilterData().categoryBits == 0xBEEF) {
        return -1;
    }

    m_fixture = fixture;
    m_point = point;
    m_normal = normal;
    m_fraction = fraction;

    return fraction;
}
