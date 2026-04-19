#ifndef ENGINEUTILS_H
#define ENGINEUTILS_H

#include <vector>
#include <string>
#include <cstdint>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

class EngineUtils {
public:

	// read a file into a string
	static void ReadJsonFile(const std::string& path, rapidjson::Document& out_document);
	static void AppendIntToString(std::string& str, int value);
	static bool LoadConfigFile(const std::string& file_path, rapidjson::Document& cfg_doc, bool exit_on_not_found);
	static std::vector<std::string> GetStringArray(const rapidjson::Value& val, const char* member_name);

	static std::string GetString(const rapidjson::Value& val, const char* member_name, const std::string default_value);
	static int GetInt(const rapidjson::Value& val, const char* member_name, const int default_value);
	static int GetBool(const rapidjson::Value& val, const char* member_name, bool default_value);
	static float GetFloat(const rapidjson::Value& val, const char* member_name, const float default_value);

	/*
	template <typename T>
	struct is_reference_wrapper : std::false_type {};
	template <typename T>
	struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};

	template<typename... Args>
	static void BindJson(const rapidjson::Value& actor, StringPool& str_pool, Args&&... bindings) {
		(..., [&](auto& binding) {
			const char* key = binding.first;
			if (!actor.HasMember(key)) return;

			auto& target = [&]() -> auto& {
				if constexpr (is_reference_wrapper<std::decay_t<decltype(binding.second)>>::value) {
					return binding.second.get();
				}
				else {
					return binding.second;
				}
				}();
			using T = std::decay_t<decltype(target)>;

			if constexpr (std::is_same_v<T, float>) {
				if (actor[key].IsFloat() || actor[key].IsInt()) target = actor[key].GetFloat();
			}
			else if constexpr (std::is_same_v<T, int>) {
				if (actor[key].IsInt()) target = actor[key].GetInt();
			}
			else if constexpr (std::is_same_v<T, bool>) {
				if (actor[key].IsBool()) target = actor[key].GetBool();
			}
			else if constexpr (std::is_same_v<T, StringRef>) {
				if (actor[key].IsString()) target = str_pool.Add(actor[key].GetString());
			}
			}(bindings));
	}

	template <typename T>
	static auto Bind(const char* key, T& target) { return std::make_pair(key, std::ref(target)); }

	static inline constexpr uint32_t HashAnimState(const char* str) {
		uint32_t hash = 2166136261u;
		while (*str) {
			hash ^= static_cast<uint8_t>(*str);
			hash *= 16777619u;
			str++;
		}
		return hash;
	}

	static inline uint32_t HashAnimState(const std::string& str) {
		return HashAnimState(str.c_str());
	}

	static inline void ReportError(const std::string& actor_name, const luabridge::LuaException& e) {
		std::string error_message = e.what();
		std::replace(error_message.begin(), error_message.end(), '\\', '/');
		std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << "\n";
	}
	*/
};

#endif