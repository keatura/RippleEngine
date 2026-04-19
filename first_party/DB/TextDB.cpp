#include "DB/TextDB.h"

#include <iostream>
#include <filesystem>

void TextDB::Initialize()
{
	TTF_Init();
	if (std::filesystem::exists(base_filepath)) {
		size_t num_fonts = 0;
		for (const auto& txt_file : std::filesystem::directory_iterator(base_filepath)) {
			std::string name = (txt_file.path().stem().string());
			font_name_to_font_id[name] = num_fonts;
			font_objects[num_fonts] = {};
			font_filepaths[num_fonts] = (base_filepath + name + ".ttf").c_str();
			num_fonts++;
		}
	}
}

TTF_Font* TextDB::GetFont(const std::string& font_name, size_t font_size)
{
	auto itr = font_name_to_font_id.find(font_name);
	if (itr == font_name_to_font_id.end()) {
		std::cout << "error: font " << font_name << " missing";
		exit(0);
	}
	size_t font_id = itr->second;

	std::unordered_map<FontSize, TTF_Font*>& font_sizes = font_objects[font_id];

	if (font_sizes.find(font_size) == font_sizes.end()) // we dont have it in that size
		font_sizes[font_size] = TTF_OpenFont(font_filepaths[font_id].c_str(), static_cast<int>(font_size));
	return font_sizes[font_size];
}
