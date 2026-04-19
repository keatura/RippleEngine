//#include <algorithm>
//#include <cstddef>
//#include "SpatialMap.h"
//
//void SpatialMap::SetXYBitshifts(uint32_t x_bitshift, uint32_t y_bitshift)
//{
//	this->x_bitshift = x_bitshift;
//	this->y_bitshift = y_bitshift;
//}
//
//void SpatialMap::Clear() {
//	spatial_map.ClearValues();
//}
//
//void SpatialMap::Insert(uint32_t actor_id, fvec2 position)
//{
//	spatial_map[PosToHash(position)].push_back(actor_id);
//}
//
//void SpatialMap::UpdateActor(uint32_t actor_id, fvec2 old_pos, fvec2 new_pos) 
//{
//	uint64_t old_hash = PosToHash(old_pos);
//	uint64_t new_hash = PosToHash(new_pos);
//
//	if (old_hash == new_hash) return;
//
//	std::vector<uint32_t>* old_cell = spatial_map.Find(old_hash);
//	if (old_cell) {
//		auto it = std::find(old_cell->begin(), old_cell->end(), actor_id);
//		if (it != old_cell->end()) {
//			*it = old_cell->back();
//			old_cell->pop_back();
//		}
//
//		if (old_cell->empty()) {
//			spatial_map.Erase(old_hash); // drop the bucket if unused
//		}
//	}
//
//	spatial_map[new_hash].push_back(actor_id);
//}
//
//inline constexpr ivec2 nearby_offsets[9] = {
//	{-1, 1}, {0,1}, {1,1},
//	{-1,0}, {0,0}, {1,0},
//	{-1,-1},{0,-1}, {1,-1}
//};
//
//void SpatialMap::GetNearby(fvec2 position, std::vector<uint32_t>& out_actors) {
//	out_actors.clear();
//
//	uivec2 base_pos{
//		static_cast<uint32_t>(static_cast<double>(position.x) + WORLD_OFFSET) >> x_bitshift,
//		static_cast<uint32_t>(static_cast<double>(position.y) + WORLD_OFFSET) >> y_bitshift,
//	};
//
//	const std::vector<uint32_t>* nearby_cells[9];
//	size_t cell_count = 0;
//	size_t total_actors = 0;
//
//	for (uint32_t curr_offset_idx = 0; curr_offset_idx < 9; curr_offset_idx++) {
//		uint32_t curr_x = base_pos.x + nearby_offsets[curr_offset_idx].x;
//		uint32_t curr_y = base_pos.y + nearby_offsets[curr_offset_idx].y;
//
//		uint64_t hash = (static_cast<uint64_t>(curr_x) << 32) | curr_y;
//
//		auto* cell_actors = spatial_map.Find(hash);
//
//		if (cell_actors && !cell_actors->empty()) {
//			nearby_cells[cell_count++] = cell_actors;
//			total_actors += cell_actors->size();
//		}
//	}
//
//	if (total_actors == 0) return;
//
//	out_actors.reserve(total_actors);
//
//	for (size_t i = 0; i < cell_count; ++i) {
//		out_actors.insert(out_actors.end(), nearby_cells[i]->begin(), nearby_cells[i]->end());
//	}
//}
