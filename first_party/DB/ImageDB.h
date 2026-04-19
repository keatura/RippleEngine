#ifndef IMAGEDB_H
#define IMAGEDB_H

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

#include "Math/FastVector.h"

typedef struct _MonoString MonoString;

class ImageDB
{
private:
	static inline SDL_Renderer* renderer = nullptr;
	static inline std::vector<std::string> names;
	static inline std::unordered_map<std::string, uint32_t> texture_map;
	static int GetImageCount();
	static MonoString* GetImageName(int index);



	static inline int atlas_w = 1.0f;
	static inline int atlas_h = 1.0f;
public:
	static inline std::vector<fvec2> image_dimensions;
	static inline std::vector<SDL_FRect> image_src_rects;
	static inline SDL_Texture* image_atlas = nullptr;


	static inline std::string base_filepath = "resources/images/"; 
	static void Initialize(SDL_Renderer* renderer);
	static void SaveAtlasToPNG(std::string filename);
    static void CreateDefaultParticleTextureWithName(const std::string& name);
	static uint32_t GetTextureIdx(const std::string& img_name);
	static uint32_t GetTextureIdx(std::string_view img_name_view);
	static std::string& GetTextureName(uint32_t texture_idx);
};

#endif
