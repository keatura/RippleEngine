//#pragma once
//#include <cstdint>
//#include <vector>
//#include "Math/FastVector.h"
//#include "Utils/FlatMap.h"
//
//class SpatialMap
//{
//private:
//	FlatMap<uint64_t, std::vector<uint32_t>> spatial_map;
//	uint32_t x_bitshift;
//	uint32_t y_bitshift;
//
//	
//
//	inline uint64_t PosToHash(fvec2 pos) const {
//		// this should map all reasonable floats to uint32_t values bijectively, should be better than using stl floor
//		uint32_t uint_x = static_cast<uint32_t>(static_cast<double>(pos.x) + WORLD_OFFSET) >> x_bitshift;
//		uint32_t uint_y = static_cast<uint32_t>(static_cast<double>(pos.y) + WORLD_OFFSET) >> y_bitshift;
//
//		return (static_cast<uint64_t>(uint_x) << 32) | uint_y;
//	}
//public:
//	void Clear();
//	void SetXYBitshifts(uint32_t x_bitshift, uint32_t y_bitshift);
//	void Insert(uint32_t actor_id, fvec2 position); 
//	void UpdateActor(uint32_t actor_id, fvec2 old_pos, fvec2 new_pos);
//	void GetNearby(fvec2 position, std::vector<uint32_t>& out_actors);
//};
//
