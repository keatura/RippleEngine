//#pragma once
//#include <cstdint>
//#include <limits>
//#include <vector>
//#include <string>
//
//namespace ECS {
//	using ComponentTypeID = uint32_t;
//	using InnerID = uint32_t;
//	using ActorID = uint32_t;
//
//	static inline constexpr uint32_t C_INVALID_IDX = std::numeric_limits<uint32_t>::max();
//
//	enum ComponentState : uint8_t {
//		ENABLED,
//		DISABLED,
//		INVALID
//	};
//
//	struct ScriptConfig {
//		const char* script_name;
//		bool has_start = false;
//		bool has_update = false;
//		bool has_late_update = false;
//	};
//
//	struct ComponentIndexer {
//		StringRef name_ref;
//		ComponentTypeID ctype_id = C_INVALID_IDX;
//		InnerID inner_id = C_INVALID_IDX;
//	};
//
//	struct LifecycleMask {
//		std::vector<uint64_t> chunks;
//
//		void Initialize(size_t num_chunks) {
//			chunks.assign(num_chunks, 0);
//		}
//
//		void SetBit(uint32_t type_id) {
//			size_t chunk_idx = type_id / 64;
//			size_t bit_offset = type_id % 64;
//			chunks[chunk_idx] |= (1ULL << bit_offset);
//		}
//
//		inline bool HasBit(uint32_t type_id) const {
//			static int mask = 0b1;
//			size_t chunk_idx = (type_id / 64);
//			size_t bit_offset = type_id % 64;
//			return mask & (chunks[chunk_idx] >> bit_offset);
//		}
//
//		inline uint64_t GetIntersection(uint64_t actor_chunk, size_t chunk_idx) const {
//			return actor_chunk & chunks[chunk_idx];
//		}
//	};
//}