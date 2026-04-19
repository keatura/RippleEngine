#ifndef SIGNATUREREGISTRY_H
#define SIGNATUREREGISTRY_H

#include "ECSTypes.h"
#include <vector>
#include <cassert>
namespace ECS {
    class SignatureRegistry
    {
    private:
        size_t num_component_types = 0;
        size_t chunks_per_actor = 0;
        std::vector<uint64_t> flat_signatures;
    public:

        inline void Initialize(size_t total_component_types) {
            num_component_types = total_component_types;
            chunks_per_actor = (num_component_types + 63) / 64;
        }

        inline void AllocateActor(ActorID actor_id) {
            size_t required_size = (actor_id + 1) * chunks_per_actor;
            if (flat_signatures.size() < required_size) {
                flat_signatures.resize(required_size, 0);
            }
        }

        inline void SetBit(ActorID actor_id, uint32_t type_id) {
            assert(type_id < num_component_types);
            size_t chunk_idx = (actor_id * chunks_per_actor) + (type_id / 64);
            size_t bit_offset = type_id % 64;
            flat_signatures[chunk_idx] |= (1ULL << bit_offset);
        }

        inline void ClearBit(ActorID actor_id, uint32_t type_id) {
            assert(type_id < num_component_types);
            size_t chunk_idx = (actor_id * chunks_per_actor) + (type_id / 64);
            size_t bit_offset = type_id % 64;
            flat_signatures[chunk_idx] &= ~(1ULL << bit_offset);
        }

        inline void ClearAllBits(ActorID actor_id) {
            size_t chunk_idx = (actor_id * chunks_per_actor);
            for (size_t i = 0; i < chunks_per_actor; ++i) {
                flat_signatures[chunk_idx + i] = 0;
            }
        }

        inline bool HasBit(ActorID actor_id, uint32_t type_id) const {
            assert(type_id < num_component_types);
            size_t chunk_idx = (actor_id * chunks_per_actor) + (type_id / 64);
            size_t bit_offset = type_id % 64;
            return (flat_signatures[chunk_idx] & (1ULL << bit_offset)) != 0;
        }

        inline size_t GetChunksPerActor() const { return chunks_per_actor; }

        inline uint64_t GetChunk(ActorID actor_id, size_t chunk_offset) const { return flat_signatures[(actor_id * chunks_per_actor) + chunk_offset]; }
    };
}

#endif