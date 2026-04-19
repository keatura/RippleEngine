//#pragma once
//#include <vector>
//#include <fstream>
//#include <iostream>
//#include "Utils/EngineUtils.h"
//
//#include "lua.hpp"
//#include "LuaBridge/LuaBridge.h"
//
//// base interface
//class IComponent {
//public:
//	virtual void Clear() = 0;
//	virtual void Resize(size_t size) = 0;
//	virtual void Save(std::ofstream& out) = 0;
//	virtual void Load(std::ifstream& in) = 0;
//	virtual ~IComponent() = default;
//};
//
//// the dense ones
//// components can either be dense or sparse->dense in style, some benefit from just going dense from the beginning
//
//template <typename T>
//class DenseComponent : public IComponent {
//public:
//	std::vector<T> data;
//	inline T& operator[](uint32_t actor_id) { return data[actor_id]; }
//	inline const T& operator[](uint32_t actor_id) const { return data[actor_id]; }
//
//	void Clear() override { data.clear(); }
//	void Resize(size_t new_size) override { data.resize(new_size); }
//	 
//	void Save(std::ofstream& out) override {
//		size_t size = data.size();
//		out.write(reinterpret_cast<const char*>(&size), sizeof(size));
//		if (size > 0) out.write(reinterpret_cast<const char*>(data.data()), size * sizeof(T));
//	}
//	void Load(std::ifstream& in) override {
//		size_t size;
//		in.read(reinterpret_cast<char*>(&size), sizeof(size));
//		data.resize(size);
//		if (size > 0) in.read(reinterpret_cast<char*>(data.data()), size * sizeof(T));
//	}
//};
//
//template <typename T>
//class SparseComponent : public IComponent {
//public:
//	std::vector<T> dense_data;
//	std::vector<uint32_t> sparse_map;
//	std::vector<uint32_t> dense_to_entity;
//
//	inline T& operator[](uint32_t actor_id) { return dense_data[sparse_map[actor_id]]; }
//	inline const T& operator[](uint32_t actor_id) const { return dense_data[sparse_map[actor_id]]; }
//	inline bool Has(uint32_t actor_id) const { return actor_id < sparse_map.size() && sparse_map[actor_id] != UINT32_MAX; }
//	inline T& Get(uint32_t actor_id) { return dense_data[sparse_map[actor_id]]; }
//	void Add(uint32_t actor_id, const T& value) {
//		if (actor_id >= sparse_map.size()) sparse_map.resize(actor_id + 1, UINT32_MAX);
//		if (sparse_map[actor_id] != UINT32_MAX) {
//			dense_data[sparse_map[actor_id]] = value;
//			return;
//		}
//		sparse_map[actor_id] = static_cast<uint32_t>(dense_data.size());
//		dense_data.push_back(value);
//		dense_to_entity.push_back(actor_id);
//	}
//
//	void Remove(uint32_t actor_id) { // swap and pop baby
//		if (!Has(actor_id)) return;
//
//		uint32_t idx = sparse_map[actor_id];
//		uint32_t last_idx = static_cast<uint32_t>(dense_data.size() - 1);
//		uint32_t last_entity = dense_to_entity[last_idx];
//
//		dense_data[idx] = dense_data[last_idx];
//		dense_to_entity[idx] = last_entity;
//		sparse_map[last_entity] = idx;
//
//		dense_data.pop_back();
//		dense_to_entity.pop_back();
//		sparse_map[actor_id] = UINT32_MAX;
//	}
//
//	void Clear() override { dense_data.clear(); sparse_map.clear(); }
//	void Resize(size_t new_size) override { sparse_map.resize(new_size, UINT32_MAX); }
//
//	void Save(std::ofstream& out) override { 
//		size_t 
//			dense_size = dense_data.size(),
//			sparse_size = sparse_map.size();
//		out.write(reinterpret_cast<const char*>(&dense_size), sizeof(dense_size));
//		out.write(reinterpret_cast<const char*>(&sparse_size), sizeof(sparse_size));
//		if (dense_size > 0) {
//			out.write(reinterpret_cast<const char*>(dense_data.data()), dense_size * sizeof(T));
//			out.write(reinterpret_cast<const char*>(dense_to_entity.data()), dense_size * sizeof(uint32_t));
//		}
//		if (sparse_size > 0) out.write(reinterpret_cast<const char*>(sparse_map.data()), sparse_size * sizeof(uint32_t));
//	}
//
//	void Load(std::ifstream& in) override {
//		size_t dense_size, sparse_size;
//		in.read(reinterpret_cast<char*>(&dense_size), sizeof(dense_size));
//		in.read(reinterpret_cast<char*>(&sparse_size), sizeof(sparse_size));
//		dense_data.resize(dense_size);
//		sparse_map.resize(sparse_size);
//		dense_to_entity.resize(dense_size);
//		if (dense_size > 0) {
//			in.read(reinterpret_cast<char*>(dense_data.data()), dense_size * sizeof(T));
//			in.read(reinterpret_cast<char*>(dense_to_entity.data()), dense_size * sizeof(uint32_t));
//		}
//		if (sparse_size > 0) in.read(reinterpret_cast<char*>(sparse_map.data()), sparse_size * sizeof(uint32_t));
//	}
//};