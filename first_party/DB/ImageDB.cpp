#include "DB/ImageDB.h"

#include "mono/metadata/appdomain.h"
#include "mono/metadata/object.h"

#include <filesystem>
#include <iostream>

int ImageDB::GetImageCount()
{
	return static_cast<int>(image_dimensions.size());
}

MonoString* ImageDB::GetImageName(int index)
{
	return mono_string_new(mono_domain_get(), names[index].c_str());
}

void ImageDB::Initialize(SDL_Renderer* renderer) {
	mono_add_internal_call("Scripts.Image::GetImageCount", &ImageDB::GetImageCount);
	mono_add_internal_call("Scripts.Image::GetImageName", &ImageDB::GetImageName);

	ImageDB::renderer = renderer;
	float max_w = 0, max_h = 0;
	std::vector<SDL_Texture*> textures;

	if (std::filesystem::exists(base_filepath)) {
		for (const auto& img_file : std::filesystem::directory_iterator(base_filepath)) {
			std::string name = (img_file.path().stem().string());
			SDL_Texture* curr_texture = IMG_LoadTexture(renderer, (base_filepath + name + ".png").c_str());
			textures.push_back(curr_texture);
			texture_map[name] = static_cast<uint32_t>(textures.size() - 1);
			names.push_back(name);

			float w = 0, h = 0;
			SDL_GetTextureSize(curr_texture, &w, &h);
			image_dimensions.emplace_back(w, h);

			if (max_w < w) max_w = w;
			if (max_h < h) max_h = h;
		}
	}
	while (atlas_w < max_w) atlas_w *= 2;
	while (atlas_h < max_h) atlas_h *= 2;
	int rect_area = atlas_w * atlas_h;
	int area_required = rect_area * textures.size();
	int atlas_side_length = 1;
	while (atlas_side_length * atlas_side_length * rect_area < area_required) atlas_side_length *= 2;

	image_atlas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, atlas_w* atlas_side_length, atlas_h * atlas_side_length);

	SDL_SetRenderTarget(renderer, image_atlas);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Transparent background
	SDL_RenderClear(renderer);

	// create the atlas
	for (size_t i = 0; i < textures.size(); i++) {
		int x_idx = i % atlas_side_length;
		int y_idx = i / atlas_side_length;

		SDL_FRect offset_rect = {
			(float)(x_idx * atlas_w),
			(float)(y_idx * atlas_h),
			image_dimensions[i].x,
			image_dimensions[i].y
		};

		image_src_rects.push_back(offset_rect);

		SDL_RenderTexture(renderer, textures[i], nullptr, &offset_rect);
	}

	SDL_SetRenderTarget(renderer, NULL);

	for (size_t i = 0; i < textures.size(); i++) {
		SDL_Texture* texture = textures[i];
		SDL_DestroyTexture(texture);
	}

	CreateDefaultParticleTextureWithName("");
}

void ImageDB::SaveAtlasToPNG(std::string filename) {
	SDL_Texture* target = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, image_atlas);

	float w, h;
	SDL_GetTextureSize(target, &w, &h);
	SDL_Surface* surface = SDL_RenderReadPixels(renderer, nullptr);

	IMG_SavePNG(surface, (base_filepath + filename).c_str());

	SDL_DestroySurface(surface);
	SDL_SetRenderTarget(renderer, target);
}

void ImageDB::CreateDefaultParticleTextureWithName(const std::string& name)
{
	//if (texture_map.find(name) != texture_map.end()) { // only create once
	//	return;
 //   }

	//SDL_Surface* surface = SDL_CreateSurface(8, 8, SDL_PIXELFORMAT_RGBA8888);
	//const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(surface->format);

 //   Uint32 white_color = SDL_MapRGBA(details, NULL, 255, 255, 255, 255);
 //   SDL_FillSurfaceRect(surface, NULL, white_color);

	//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	//SDL_DestroySurface(surface);

	//float w = 0, h = 0;
	//SDL_GetTextureSize(texture, &w, &h);
	//image_dimensions.emplace_back(w, h);
	//textures.push_back(texture);
	//texture_map[name] = static_cast<uint32_t>(textures.size() - 1);
	//names.push_back(name);
}

uint32_t ImageDB::GetTextureIdx(const std::string& img_name)
{
	if (texture_map.find(img_name) == texture_map.end()) {
		std::cout << "error: missing image " << img_name;
		exit(0);
	}

	return texture_map[img_name];
}

uint32_t ImageDB::GetTextureIdx(std::string_view img_name_view)
{
	return GetTextureIdx(std::string(img_name_view));
}

std::string& ImageDB::GetTextureName(uint32_t texture_idx)
{
	return names[texture_idx];
}