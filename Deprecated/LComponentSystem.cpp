//#include "LComponentSystem.h"
//
////#define NDEBUG
//
//namespace ECS {
//
//    LComponentPool::LComponentPool(lua_State* state, const char* component_name) : instance_metatable(state)
//    {
//        base_component_ref = luabridge::getGlobal(state, component_name);
//        lua_state = state;
//
//        instance_metatable = luabridge::newTable(lua_state);
//        instance_metatable["__index"] = base_component_ref;
//
//        // initialize config info
//        config_info = ScriptConfig{ component_name };
//        config_info.has_start = base_component_ref["OnStart"].isFunction();
//        config_info.has_update = base_component_ref["OnUpdate"].isFunction();
//        config_info.has_late_update = base_component_ref["OnLateUpdate"].isFunction();
//    }
//
//    void LComponentPool::InjectJsonData(const rapidjson::Value& component_json, InnerID inner_id)
//    {
//        luabridge::LuaRef ref_to_bind = dense_data[inner_id];
//
//        for (auto itr = component_json.MemberBegin(); itr != component_json.MemberEnd(); ++itr) {
//            const std::string field_name = itr->name.GetString();
//            if (field_name == "type") continue;
//
//            assert(!base_component_ref[field_name].isNil()); // sanity check
//
//            if (itr->value.IsString()) {
//                ref_to_bind[field_name] = itr->value.GetString();
//            }
//            else if (itr->value.IsNumber()) {
//                if (itr->value.IsInt()) {
//                    ref_to_bind[field_name] = itr->value.GetInt();
//                }
//                else {
//                    ref_to_bind[field_name] = itr->value.GetFloat();
//                }
//            }
//            else if (itr->value.IsBool()) {
//                ref_to_bind[field_name] = itr->value.GetBool();
//            }
//        }
//    }
//
//    InnerID LComponentPool::Add(ActorID actor_id)
//    {
//        luabridge::LuaRef instance_table = luabridge::newTable(lua_state);
//
//        instance_table.push(lua_state);
//        instance_metatable.push(lua_state);
//        lua_setmetatable(lua_state, -2);
//        lua_pop(lua_state, 1);
//        
//        InnerID new_inner_id = dense_data.size();
//        dense_data.push_back(instance_table);
//        dense_to_entity.push_back(actor_id);
//        
//        entity_to_dense[actor_id].push_back(new_inner_id);
//        return new_inner_id;
//    }
//
//    void LComponentPool::Remove(ActorID actor_id, InnerID inner_id)
//    {
//        assert(inner_id < dense_data.size());
//
//        size_t last_idx = dense_data.size() - 1;
//
//        if (inner_id != last_idx) {
//            ActorID last_actor = dense_to_entity[last_idx];
//            luabridge::LuaRef last_lua_ref = dense_data[last_idx];
//
//            dense_to_entity[inner_id] = last_actor;
//            dense_data[inner_id] = last_lua_ref;
//            
//            // replace in last actor's thingy
//            std::vector<uint32_t>* last_actor_instances = entity_to_dense.Find(last_actor);
//            if (last_actor_instances) { 
//                for (uint32_t& id : *last_actor_instances) {
//                    if (id == last_idx) {
//                        id = inner_id;
//                        break;
//                    }
//                }
//            }
//        }
//
//        std::vector<uint32_t>& curr_instances = entity_to_dense[actor_id];
//
//        for (size_t i = 0; i < curr_instances.size(); ++i) {
//            if (curr_instances[i] == inner_id) {
//                // Swap with the last element and pop!
//                curr_instances[i] = curr_instances.back();
//                curr_instances.pop_back();
//                break;
//            }
//        }
//
//        if (curr_instances.empty()) {  
//            entity_to_dense.Erase(actor_id);
//        }
//
//        dense_data.pop_back();
//        dense_to_entity.pop_back();
//    }
//
//    void LComponentPool::RemoveAll(ActorID actor_id)
//    {
//        std::vector<InnerID>* map_entry = entity_to_dense.Find(actor_id);
//        if (!map_entry) return;
//
//        std::vector<InnerID> components_to_delete = *map_entry;
//
//        for (InnerID curr_id : components_to_delete) {
//            Remove(actor_id, curr_id);
//        }
//    }
//
//    luabridge::LuaRef LComponentPool::GetRef(InnerID inner_id)
//    {
//        assert(inner_id < dense_data.size());
//        luabridge::LuaRef ref_to_return = dense_data[inner_id];
//        if (dense_data[inner_id]["enabled"] == false) return luabridge::LuaRef(lua_state);
//        return dense_data[inner_id];
//    }
//
//    std::vector<InnerID> LComponentPool::GetAllIndices(ActorID actor_id)
//    {
//        std::vector<InnerID>* indices = entity_to_dense.Find(actor_id);
//        assert(indices != nullptr);
//        return *indices;
//    }
//
//    std::vector<luabridge::LuaRef> LComponentPool::GetAllRefs(ActorID actor_id)
//    {
//        std::vector<InnerID>* indices = entity_to_dense.Find(actor_id);
//        assert(indices != nullptr);
//        std::vector<luabridge::LuaRef> to_ret;
//        for (auto& idx : *indices)
//            to_ret.push_back(dense_data[idx]);
//        return to_ret;
//    }
//
//    bool LComponentPool::IsEnabled(InnerID inner_id)
//    {
//        return this->GetRef(inner_id)["enabled"] == true;
//    }
//
//    void LComponentPool::Enable(InnerID inner_id)
//    {
//        assert(inner_id < dense_data.size());
//        this->GetRef(inner_id)["enabled"] = true;
//    }
//
//    inline void LComponentPool::Disable(InnerID inner_id)
//    {
//        assert(inner_id < dense_data.size());
//        this->GetRef(inner_id)["enabled"] = false;
//    }
//
//    void LComponentSystem::RemoveActor(ActorID actor_id)
//    {
//        for (uint32_t pool_idx = 0; pool_idx < num_components; ++pool_idx) {
//            if (actor_signatures.HasBit(actor_id, pool_idx)) {
//                components[pool_idx].RemoveAll(actor_id);
//            }
//        }
//
//        actor_signatures.ClearAllBits(actor_id);
//    }
//
//    void LComponentSystem::InitializeAfterRegisters()
//    { // call this after registering all components
//        actor_signatures.Initialize(num_components);
//
//        size_t num_chunks = actor_signatures.GetChunksPerActor();
//        on_start_mask.Initialize(num_chunks);
//        on_update_mask.Initialize(num_chunks);
//        on_late_update_mask.Initialize(num_chunks);
//
//        for (uint32_t type_id = 0; type_id < num_components; ++type_id) {
//            LComponentPool& curr_component = components[type_id];
//
//            if (curr_component.HasStart()) on_start_mask.SetBit(type_id);
//            if (curr_component.HasUpdate()) on_update_mask.SetBit(type_id);
//            if (curr_component.HasLateUpdate()) on_late_update_mask.SetBit(type_id);
//        }
//    }
//
//    ComponentIndexer LComponentSystem::AddComponent(ActorID actor_id, const rapidjson::Value& component_json, const char* key)
//    {
//        assert(!component_json["type"].IsNull() && component_json["type"].IsString());
//        std::string component_type = component_json["type"].GetString();
//
//        auto pool_itr = name_to_idx.find(component_type);
//        if (pool_itr == name_to_idx.end()) { // error handling, check to make sure component actually exists
//            std::cout << "error: failed to locate component " << component_type;
//            exit(0);
//        }
//
//        actor_signatures.AllocateActor(actor_id);
//
//        size_t pool_idx = pool_itr->second;
//        LComponentPool& component_list = components[pool_idx];
//        InnerID new_inner_id = component_list.Add(actor_id);
//
//        luabridge::LuaRef new_comp_ref = component_list.GetRef(new_inner_id);
//
//        new_comp_ref["key"] = key;
//        new_comp_ref["enabled"] = true;
//        component_list.InjectJsonData(component_json, new_inner_id);
//        actor_signatures.SetBit(actor_id, pool_idx);
//
//        return ComponentIndexer{ {}, static_cast<uint32_t>(pool_idx), new_inner_id};
//    }
//
//    ComponentIndexer LComponentSystem::AddComponentAtRuntime(const ActorID actor_id, const std::string& comp_type, const char* comp_key)
//    {
//        auto pool_itr = name_to_idx.find(comp_type);
//        if (pool_itr == name_to_idx.end()) { // error handling, check to make sure component actually exists
//            std::cout << "error: failed to locate component " << comp_type;
//            exit(0);
//        }
//
//        size_t pool_idx = pool_itr->second;
//        LComponentPool& component_list = components[pool_idx];
//        InnerID new_inner_id = component_list.Add(actor_id);
//
//        luabridge::LuaRef new_comp_ref = component_list.GetRef(new_inner_id);
//
//        new_comp_ref["key"] = comp_key;
//        new_comp_ref["enabled"] = true;
//        actor_signatures.SetBit(actor_id, pool_idx);
//
//        return ComponentIndexer{ {}, static_cast<uint32_t>(pool_idx), new_inner_id };
//    }
//
//    luabridge::LuaRef LComponentSystem::GetComponentByKey(ComponentTypeID ctype_id, InnerID inner_id) {
//        return components[ctype_id].GetRef(inner_id);
//    }
//
//    luabridge::LuaRef LComponentSystem::GetComponentByType(ActorID actor_id, std::string type_name)
//    {
//        auto pool_itr = name_to_idx.find(type_name);
//        if (pool_itr == name_to_idx.end()) { // error handling, check to make sure component actually exists
//            return luabridge::LuaRef(lua_state); // nil return if none exist
//        }
//
//        size_t pool_idx = pool_itr->second;
//        LComponentPool& component_list = components[pool_idx];
//        std::vector<luabridge::LuaRef> comp_refs = component_list.GetAllRefs(actor_id);
//
//        if (comp_refs.size() == 0) return luabridge::LuaRef(lua_state);
//        if (comp_refs.size() == 1) return comp_refs[0];
//
//        // find lexicographically first component 
//        size_t smallest_index = 0;
//        for (size_t ref_idx = 0; ref_idx < comp_refs.size(); ref_idx++) {
//            if (comp_refs[ref_idx]["key"].tostring() < comp_refs[smallest_index]["key"].tostring()) // no tiebreakers because keys are unique within an actor
//                smallest_index = ref_idx;
//        }
//
//        return comp_refs[smallest_index];
//    }
//
//    luabridge::LuaRef LComponentSystem::GetAllComponentsOfType(ActorID actor_id, std::string type_name)
//    {
//        luabridge::LuaRef table_to_return = luabridge::newTable(lua_state);
//        auto pool_itr = name_to_idx.find(type_name);
//        if (pool_itr == name_to_idx.end()) { // error handling, check to make sure component actually exists
//            return table_to_return; // nil return if none exist
//        }
//
//        size_t pool_idx = pool_itr->second;
//        LComponentPool& component_list = components[pool_idx];
//        std::vector<luabridge::LuaRef> comp_refs = component_list.GetAllRefs(actor_id);
//
//        if (comp_refs.size() == 0) return table_to_return;
//
//        for (size_t i = 0; i < comp_refs.size(); i++) {
//            table_to_return[i + 1] = comp_refs[i];
//        }
//
//        return table_to_return;
//    }
//
//    void LComponentSystem::ModifyComponent(const rapidjson::Value& component_json, const ComponentIndexer& indexer) {
//        size_t pool_idx = indexer.ctype_id;
//        LComponentPool& component_list = components[indexer.ctype_id];
//
//        component_list.InjectJsonData(component_json, indexer.inner_id);
//    }
//}
