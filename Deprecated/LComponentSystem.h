//#pragma once
//
//#include <vector>
//#include <fstream>
//#include <iostream>
//#include "Utils/EngineUtils.h"
//
//#include "lua.hpp"
//#include "LuaBridge/LuaBridge.h"
//#include "FlatMap.h"
//#include "ECSTypes.h"
//#include "SignatureRegistry.h"
//
//namespace ECS {
//
//	class LComponentPool { // for use with lua stuff
//	private:
//		std::vector<luabridge::LuaRef> dense_data; // actual component storage
//		std::vector<ActorID> dense_to_entity; // for answering "what actor uses me"
//		FlatMap<ActorID, std::vector<uint32_t>> entity_to_dense;
//		luabridge::LuaRef base_component_ref = nullptr;
//		luabridge::LuaRef instance_metatable = nullptr;
//		lua_State* lua_state;
//
//		ScriptConfig config_info;
//
//	public:
//		LComponentPool(lua_State* state, const char* component_name);
//		void InjectJsonData(const rapidjson::Value& component_json, InnerID inner_id);
//		InnerID Add(ActorID actor_id);
//		void Remove(ActorID actor_id, InnerID inner_id);
//		void RemoveAll(ActorID actor_id);
//
//		inline bool HasStart() const { return config_info.has_start; }
//		inline bool HasUpdate() const { return config_info.has_update; }
//		inline bool HasLateUpdate() const { return config_info.has_late_update; }
//
//		luabridge::LuaRef GetRef(InnerID inner_id); // get specific component by inner id
//		std::vector<InnerID> GetAllIndices(ActorID actor_id); // get all component instances for an actor
//		std::vector<luabridge::LuaRef> GetAllRefs(ActorID actor_id);
//		
//		bool IsEnabled(InnerID inner_id);
//		void Enable(InnerID inner_id);
//		inline void Disable(InnerID inner_id);
//
//		~LComponentPool() = default;
//	};
//
//	class LComponentSystem
//	{
//
//	private:
//		size_t num_components = 0;
//		std::unordered_map<std::string, size_t> name_to_idx;
//		std::vector<LComponentPool> components;
//	public:
//		lua_State* lua_state = nullptr;
//		LifecycleMask on_start_mask;
//		LifecycleMask on_update_mask;
//		LifecycleMask on_late_update_mask;
//		SignatureRegistry actor_signatures;
//		void Register(lua_State* state, std::string component_name, std::string lua_file) { // call this ONCE
//			lua_state = state;
//			if (luaL_dofile(state, lua_file.c_str()) != LUA_OK) {
//				std::cout << "problem with lua file " << component_name;
//				exit(0);
//			}
//
//			name_to_idx[component_name] = num_components;
//			num_components++;
//			components.emplace_back(LComponentPool(state, component_name.c_str()));
//		}
//
//		void OnStart(ActorID actor_id, ComponentIndexer& indexer) {
//			ComponentTypeID type_id = indexer.ctype_id;
//			if (on_start_mask.HasBit(type_id)) {
//				luabridge::LuaRef ref = components[type_id].GetRef(indexer.inner_id);
//				if (ref["enabled"])
//					ref["OnStart"](ref);
//			}
//		}
//
//		void OnUpdate(ActorID actor_id, ComponentIndexer& indexer) {
//			ComponentTypeID type_id = indexer.ctype_id;
//			if (on_update_mask.HasBit(type_id)) {
//				luabridge::LuaRef ref = components[type_id].GetRef(indexer.inner_id);
//				if (ref["enabled"])
//					ref["OnUpdate"](ref);
//			}
//		}
//
//		void OnLateUpdate(ActorID actor_id, ComponentIndexer& indexer)
//		{
//			ComponentTypeID type_id = indexer.ctype_id;
//			if (on_late_update_mask.HasBit(type_id)) {
//				luabridge::LuaRef ref = components[type_id].GetRef(indexer.inner_id);
//				if (ref["enabled"])
//					ref["OnLateUpdate"](ref);
//			}
//		}
//
//		void RemoveActor(ActorID actor_id);
//		void InitializeAfterRegisters();
//		[[nodiscard]] ComponentIndexer AddComponent(ActorID actor_id, const rapidjson::Value& component_json, const char* key);
//		[[nodiscard]] ComponentIndexer AddComponentAtRuntime(const ActorID actor_id, const std::string& comp_type, const char* comp_key); // add component at runtime
//
//		luabridge::LuaRef GetComponentByKey(ComponentTypeID ctype_id, InnerID inner_id);
//		luabridge::LuaRef GetComponentByType(ActorID actor_id, std::string type_name);
//
//		luabridge::LuaRef GetAllComponentsOfType(ActorID actor_id, std::string type_name);
//
//		void ModifyComponent(const rapidjson::Value& component_json, const ComponentIndexer& indexer);
//
//		template<typename T>
//		void InjectReference(ComponentIndexer& indexer, T& obj_ref, std::string ref_name) {
//			size_t pool_idx = indexer.ctype_id;
//			LComponentPool& component_list = components[indexer.ctype_id];
//			component_list.GetRef(indexer.inner_id)[ref_name] = &obj_ref;
//		}
//	};
//}