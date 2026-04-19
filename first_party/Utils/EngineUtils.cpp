#include "Utils/EngineUtils.h"

#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <algorithm>

void EngineUtils::ReadJsonFile(const std::string& path, rapidjson::Document& out_document) 
{
	FILE* file_pointer = nullptr;
#ifdef _WIN32
	fopen_s(&file_pointer, path.c_str(), "rb");
#else
	file_pointer = fopen(path.c_str(), "rb");
#endif
	if (!file_pointer) {
		return; // Or exit(0);
	}

	static char buffer[65536];
	rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
	out_document.ParseStream(stream);
	std::fclose(file_pointer);
	if (out_document.HasParseError()) {
		rapidjson::ParseErrorCode error_code = out_document.GetParseError();
		std::cout << "error parsing json at [" << path << "]" << std::endl;
	}
}

void EngineUtils::AppendIntToString(std::string& str, int value)
{
	if (value == 0) {
		str += '0';
		return;
	}
	// world's smallest buffer lmfao
	char temp_buffer[16];
	char* tb_iterator = temp_buffer;
	bool is_value_negative = value < 0;
	if (is_value_negative) value = -value;
	while (value > 0) {
		*tb_iterator++ = (value % 10) + '0';
		value /= 10;
	}
	if (is_value_negative) *tb_iterator++ = '-';
	while (tb_iterator > temp_buffer) {
		str += *(--tb_iterator);
	}
}

bool EngineUtils::LoadConfigFile(const std::string& file_path, rapidjson::Document& cfg_doc, bool exit_on_not_found)
{
	if (!std::filesystem::exists(file_path)) {
		if (exit_on_not_found) {
			std::cout << "error: " << file_path << " missing";
			exit(0);
		}
		else {
			return false;
		}
	}
	EngineUtils::ReadJsonFile(file_path, cfg_doc);
	return true;
}

std::vector<std::string> EngineUtils::GetStringArray(const rapidjson::Value& val, const char* member_name)
{ // this sucks, only use it if youre getting something that youre only gonna use like once or twice, string vectors SUCK
	std::vector<std::string> arr;
	if (val.HasMember(member_name) && val[member_name].IsArray()) {
		const rapidjson::Value& members = val[member_name];
		for (rapidjson::SizeType i = 0; i < members.Size(); i++) {
			arr.emplace_back(members[i].GetString());
		}
	}
	return arr;
}

std::string EngineUtils::GetString(const rapidjson::Value& val, const char* member_name, const std::string default_value)
{
	if (val.HasMember(member_name) && val[member_name].IsString()) {
		return val[member_name].GetString();
	}
	return default_value;
}

int EngineUtils::GetInt(const rapidjson::Value& val, const char* member_name, const int default_value)
{
	if (val.HasMember(member_name) && val[member_name].IsInt()) {
		return val[member_name].GetInt();
	}
	return default_value;
}

int EngineUtils::GetBool(const rapidjson::Value& val, const char* member_name, bool default_value)
{
	if (val.HasMember(member_name) && val[member_name].IsBool()) {
		return val[member_name].GetBool();
	}
	return default_value;
}

float EngineUtils::GetFloat(const rapidjson::Value& val, const char* member_name, const float default_value)
{
	if (val.HasMember(member_name) && val[member_name].IsNumber()) {
		return val[member_name].GetFloat();
	}
	return default_value;
}