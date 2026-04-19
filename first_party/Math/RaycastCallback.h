#pragma once

#include "box2d/box2d.h"
#include "box2d/b2_math.h"

class RaycastCallback : public b2RayCastCallback
{
	float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float fraction) override;

public:
	b2Fixture* m_fixture = nullptr;
	b2Vec2 m_point, m_normal;
	float m_fraction = 1.0f;
};

