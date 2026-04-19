//#pragma once
//#include <cstdint>
//#include <vector>
//#include "FastVector.h"
//
//struct MapSlot {
//    int32_t head_actor_idx = -1;
//    uint16_t blocking_count = 0;
//    char cached_view = ' ';
//    uint8_t padding;
//};
//
//class SpatialMap {
//public:
//    static constexpr uint64_t EMPTY_KEY = 0xFFFFFFFFFFFFFFFF;
//    static constexpr uint8_t CTRL_EMPTY = 0xFF;
//
//    std::vector<uint64_t> keys;
//    std::vector<size_t> last_map_index;
//    std::vector<MapSlot> data;
//    std::vector<int32_t> col_next;
//    std::vector<uint8_t> ctrl;
//
//    uint64_t mask = 0;
//    size_t total_slots_used = 0;
//    size_t max_load_threshold = 0;
//
//    void Initialize(uint32_t num_actors, const std::vector<fvec2>& pos, const std::vector<uint8_t>& blocking) {
//        this->clear();
//        size_t start_size = std::max((size_t)1024, (size_t)num_actors * 2);
//        size_t p2 = 1024;
//        while (p2 < start_size) p2 *= 2;
//        start_size = p2;
//
//        /*
//
//		col_next.assign(num_actors, -1);
//
//		last_map_index.assign(num_actors, 0);
//        ctrl.assign(start_size, SpatialMap::CTRL_EMPTY);
//        keys.assign(start_size, SpatialMap::EMPTY_KEY);
//        data.assign(start_size, MapSlot());
//
//        mask = start_size - 1;
//        total_slots_used = 0;
//        max_load_threshold = (size_t)(start_size * 0.875f);
//
//
//        for (uint32_t actor = 0; actor < num_actors; actor++) {
//            uint64_t pkey = GetPackedKey(ivec2(movement[actor].position));
//            size_t idx = FindIndex(pkey);
//
//            if (keys[idx] == SpatialMap::EMPTY_KEY) {
//                uint64_t hash = SpatialMap::MixHash(pkey);
//                ctrl[idx] = static_cast<uint8_t>(hash >> 57);
//                keys[idx] = pkey;
//                total_slots_used++;
//            }
//            last_map_index[actor] = idx;
//            LinkActorToCell(actor, data[idx], blocking[actor]);
//        }
//        */
//    }
//
//    static inline uint64_t MixHash(uint64_t k) {
//        k *= 0x2d358dccaa6c78a5ULL;
//        k ^= k >> 32;
//        k *= 0x8bb84b93962eacc9ULL;
//        k ^= k >> 32;
//        return k;
//    }
//
//    static inline uint64_t GetPackedKey(ivec2 pos) {
//        return *reinterpret_cast<uint64_t*>(&pos);
//    }
//
//    inline size_t FindIndex(uint64_t packed_key) const {
//        uint64_t hash = MixHash(packed_key);
//        size_t idx = hash & mask;
//        uint8_t h2 = static_cast<uint8_t>(hash >> 57);
//        if (h2 == CTRL_EMPTY) h2 = 0x7F; // Disambiguate from empty
//
//        while (true) {
//            // Check 1
//            if (ctrl[idx] == h2 && keys[idx] == packed_key) return idx;
//            if (ctrl[idx] == CTRL_EMPTY) return idx;
//            idx = (idx + 1) & mask;
//
//            // Check 2
//            if (ctrl[idx] == h2 && keys[idx] == packed_key) return idx;
//            if (ctrl[idx] == CTRL_EMPTY) return idx;
//            idx = (idx + 1) & mask;
//
//            // Check 3
//            if (ctrl[idx] == h2 && keys[idx] == packed_key) return idx;
//            if (ctrl[idx] == CTRL_EMPTY) return idx;
//            idx = (idx + 1) & mask;
//
//            // Check 4
//            if (ctrl[idx] == h2 && keys[idx] == packed_key) return idx;
//            if (ctrl[idx] == CTRL_EMPTY) return idx;
//            idx = (idx + 1) & mask;
//        }
//    }
//
//    inline void LinkActorToCell(uint32_t idx, MapSlot& slot, int8_t is_blocking) {
//        col_next[idx] = slot.head_actor_idx;
//        slot.head_actor_idx = idx;
//        if (is_blocking) slot.blocking_count++;
//    }
//    inline void UnlinkActorFromCell(uint32_t idx, MapSlot& slot, int8_t is_blocking) {
//        if (slot.head_actor_idx == (int32_t)idx) {
//            slot.head_actor_idx = col_next[idx];
//        }
//        else {
//            int32_t curr = slot.head_actor_idx;
//            while (curr != -1 && col_next[curr] != (int32_t)idx) {
//                curr = col_next[curr];
//            }
//            if (curr != -1) {
//                col_next[curr] = col_next[idx];
//            }
//        }
//        if (is_blocking) slot.blocking_count--;
//    }
//    inline void UpdateSlotView(MapSlot& slot, const std::vector<char>& actor_views) {
//        int32_t curr = slot.head_actor_idx;
//        if (curr == -1) {
//            slot.cached_view = ' ';
//            return;
//        }
//        if (col_next[curr] == -1) {
//            slot.cached_view = actor_views[curr];
//            return;
//        }
//
//        uint32_t max_idx = 0;
//        int32_t max_actor = -1;
//        while (curr != -1) {
//            if ((uint32_t)curr >= max_idx) {
//                max_idx = (uint32_t)curr;
//                max_actor = curr;
//            }
//            curr = col_next[curr];
//        }
//        if (max_actor != -1) slot.cached_view = actor_views[max_actor];
//    }
//
//    void Resize() {
//        size_t old_size = keys.size();
//        size_t new_size = old_size * 2;
//
//        std::vector<uint8_t> old_ctrl = std::move(ctrl);
//        std::vector<uint64_t> old_keys = std::move(keys);
//        std::vector<MapSlot> old_data = std::move(data);
//
//        ctrl.assign(new_size, SpatialMap::CTRL_EMPTY);
//        keys.assign(new_size, SpatialMap::EMPTY_KEY);
//        data.assign(new_size, MapSlot());
//
//        mask = new_size - 1;
//        total_slots_used = 0;
//        max_load_threshold = (size_t)(new_size * 0.875f);
//
//        for (size_t i = 0; i < old_size; i++) {
//            if (old_ctrl[i] == SpatialMap::CTRL_EMPTY) continue;
//
//            uint64_t pkey = old_keys[i];
//            size_t new_idx = FindIndex(pkey);
//
//            uint64_t hash = SpatialMap::MixHash(pkey);
//            ctrl[new_idx] = static_cast<uint8_t>(hash >> 57);
//            keys[new_idx] = pkey;
//            data[new_idx] = old_data[i];
//            total_slots_used++;
//
//            int32_t curr = data[new_idx].head_actor_idx;
//            while (curr != -1) {
//                last_map_index[curr] = new_idx;
//                curr = col_next[curr];
//            }
//        }
//    }
//
//    void clear() {
//		last_map_index.clear();
//        ctrl.clear();
//        keys.clear();
//        data.clear();
//		col_next.clear();
//        mask = 0;
//        total_slots_used = 0;
//        max_load_threshold = 0;
//    }
//};