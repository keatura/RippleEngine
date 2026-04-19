#pragma once

#include "box2d/box2d.h"
#include "box2d/b2_math.h"
#include <vector>

class MultiRaycastCallback : public b2RayCastCallback
{

	float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float fraction) override;

public:
	struct HitInfo {
		b2Fixture* fixture;
		b2Vec2 point;
		b2Vec2 normal;
		float fraction;

		bool operator<(const HitInfo& other) const {
			return fraction < other.fraction;
        }
	};

    std::vector<HitInfo> hits;
};
