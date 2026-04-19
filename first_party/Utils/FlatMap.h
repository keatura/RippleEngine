#ifndef FLATMAP_H
#define FLATMAP_H

#include <vector>
#include <cstdint>
#include <type_traits>
#include <cstddef>

template <typename K, typename V>
class FlatMap
{
private:
	std::vector<K> dense_keys;
	std::vector<V> dense_values;
	std::vector<uint32_t> sparse_buckets;

	size_t active_entries = 0;

	size_t capacity_shift = 64; // for fibonacci bitshifting

	static inline constexpr int SHIFT_CONSTANT = 58; // 64 - log2(64)
	static constexpr uint32_t EMPTY_SLOT = 0xFFFFFFFF;
	static constexpr uint64_t GOLDEN_RATIO = 11400714819323198485ULL; // :D

	void GrowAndRehash() {
		size_t new_capacity;
		if (sparse_buckets.empty()) {
			new_capacity = 64;
			capacity_shift = SHIFT_CONSTANT;
		}
		else {
			new_capacity = sparse_buckets.size() * 2;
			capacity_shift--;
		}

		sparse_buckets.assign(new_capacity, EMPTY_SLOT);
		uint64_t mask = new_capacity - 1;

		for (uint32_t i = 0; i < active_entries; ++i) {
			uint64_t hash = dense_keys[i] * GOLDEN_RATIO;
			size_t idx = hash >> capacity_shift;

			uint32_t tag = (hash >> 56) & 0xFE;

			while (sparse_buckets[idx] != EMPTY_SLOT) {
				idx = (idx + 1) & mask;
			}

			sparse_buckets[idx] = (tag << 24) | (i & 0x00FFFFFF);
		}
	}

public:
	FlatMap() {
		GrowAndRehash();
	}

	V* Find(K key) {
		if (sparse_buckets.empty()) return nullptr; // bleh, you get nothing

		uint64_t hash = key * GOLDEN_RATIO;
		uint64_t mask = sparse_buckets.size() - 1;
		size_t idx = hash >> capacity_shift;

		uint32_t target_tag = (hash >> 56) & 0xFE; // same trick so no 0xFF

		while (sparse_buckets[idx] != EMPTY_SLOT) {
			uint32_t bucket_idx = sparse_buckets[idx];

			if ((bucket_idx >> 24) == target_tag) { // compare tag first so you dont have to always compare the whole dense key
				uint32_t entry_idx = bucket_idx & 0x00FFFFFF;
				if (dense_keys[entry_idx] == key) {
					return &dense_values[entry_idx];
				}
			}
			idx = (idx + 1) & mask;
		}
		return nullptr;
	}

	V& operator[](K key) {
		// try to find
		V* existing = Find(key);
		if (existing) return *existing;

		// doesn't exist make new bucket, resize
		if (active_entries >= sparse_buckets.size() * 0.7f) // the only double in the entire codebase (i think) rests here, make a wish!
			GrowAndRehash();

		// find an empty slot
		uint64_t hash = key * GOLDEN_RATIO;
		uint64_t mask = sparse_buckets.size() - 1;
		size_t idx = hash >> capacity_shift;
		uint32_t target_tag = (hash >> 56) & 0xFE; // tag to use

		while (sparse_buckets[idx] != EMPTY_SLOT)
			idx = (idx + 1) & mask;

		// insert new bucket
		uint32_t new_entry_idx = static_cast<uint32_t>(active_entries);
		sparse_buckets[idx] = (target_tag << 24) | (new_entry_idx & 0x00FFFFFF);
		if (active_entries < dense_keys.size()) {
			dense_keys[active_entries] = key;
		}
		else {
			dense_keys.push_back(key);
			dense_values.push_back(V{});
		}
		active_entries++;
		return dense_values[new_entry_idx];
	}

	bool Erase(K key) {
		if (sparse_buckets.empty() || active_entries == 0) return false;

		uint64_t hash = key * GOLDEN_RATIO;
		uint64_t mask = sparse_buckets.size() - 1;
		size_t sparse_idx = hash >> capacity_shift;
		uint32_t target_tag = (hash >> 56) & 0xFE;

		// find key in sparse array
		while (sparse_buckets[sparse_idx] != EMPTY_SLOT) {
			uint32_t bucket_idx = sparse_buckets[sparse_idx];

			if ((bucket_idx >> 24) == target_tag) { // maybe match, look further to see if key actually found
				uint32_t dense_idx = bucket_idx & 0x00FFFFFF;

				if (dense_keys[dense_idx] == key) { // hurrah! key found!

					uint32_t last_dense_idx = static_cast<uint32_t>(active_entries - 1);

					if (dense_idx != last_dense_idx) { // same deal as before with the erasy algorithm
						dense_keys[dense_idx] = dense_keys[last_dense_idx];
						std::swap(dense_values[dense_idx], dense_values[last_dense_idx]);

						uint64_t moved_hash = dense_keys[dense_idx] * GOLDEN_RATIO;
						uint32_t moved_tag = (moved_hash >> 56) & 0xFE;
						size_t moved_bucket_idx = moved_hash >> capacity_shift;

						while ((sparse_buckets[moved_bucket_idx] & 0x00FFFFFF) != last_dense_idx) {
							moved_bucket_idx = (moved_bucket_idx + 1) & mask; 
						}

						sparse_buckets[moved_bucket_idx] = (moved_tag << 24) | (dense_idx & 0x00FFFFFF);
					}

					active_entries--;

					size_t hole_bucket_idx = sparse_idx;
					size_t next_bucket_idx = (hole_bucket_idx + 1) & mask;

					while (sparse_buckets[next_bucket_idx] != EMPTY_SLOT) {
						uint32_t next_dense_idx = sparse_buckets[next_bucket_idx] & 0x00FFFFFF;
						uint64_t next_hash = dense_keys[next_dense_idx] * GOLDEN_RATIO;
						size_t ideal_idx = next_hash >> capacity_shift;

						size_t dist_to_hole = (hole_bucket_idx - ideal_idx) & mask;
						size_t dist_to_next = (next_bucket_idx - ideal_idx) & mask;

						if (dist_to_hole < dist_to_next) {
							sparse_buckets[hole_bucket_idx] = sparse_buckets[next_bucket_idx];
							hole_bucket_idx = next_bucket_idx;
						}
						next_bucket_idx = (next_bucket_idx + 1) & mask;
					}

					sparse_buckets[hole_bucket_idx] = EMPTY_SLOT;
					return true;
				}
			}
			sparse_idx = (sparse_idx + 1) & mask;
		}
		return false;
	}

	void ClearValues() {
		for (size_t i = 0; i < active_entries; i++) {
			dense_values[i].clear();
		}
		active_entries = 0;
		sparse_buckets.assign(sparse_buckets.size(), EMPTY_SLOT);
	}

	auto begin() {
		return dense_values.begin();
	}

	auto end() {
		return dense_values.begin() + active_entries;
	}

	auto KeyBegin() {
		return dense_keys.begin();
	}

	auto KeyEnd() {
		return dense_keys.end();
	}
};

#endif