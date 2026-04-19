#ifndef TEXTDB_H
#define TEXTDB_H

#include <string>
#include <unordered_map>
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

class TextDB
{
	using FontID = size_t;
	using FontSize = size_t;
private:
	static inline std::unordered_map<std::string, TTF_Font*> fonts;
	static inline std::unordered_map<std::string, FontID> font_name_to_font_id;
	static inline std::unordered_map<FontID, std::string> font_filepaths;
	static inline std::unordered_map<FontID, std::unordered_map<FontSize, TTF_Font*>> font_objects;
public:
	static inline const std::string base_filepath = "resources/fonts/";
	static void Initialize();
	static TTF_Font* GetFont(const std::string& font_name, size_t font_size);
};

#endif
