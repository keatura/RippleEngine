#include "Graphics/Renderer.h"

#include "DB/TextDB.h"
#include "DB/ImageDB.h"
#include "DB/SceneDB.h"
#include "Graphics/ParticleSystem.h"
#include "Utils/EngineUtils.h"

#include "mono/metadata/object.h"
#include "mono/metadata/appdomain.h"

#include "glm/glm.hpp"

#include <iostream>
#include <algorithm>
#include "AudioManager.h"

void Renderer::Initialize(const std::string& game_title)
{
	rapidjson::Document render_cfg;
	if (EngineUtils::LoadConfigFile("resources/rendering.config", render_cfg, false)) {
		x_resolution = EngineUtils::GetInt(render_cfg, "x_resolution", 640);
		y_resolution = EngineUtils::GetInt(render_cfg, "y_resolution", 360);
		clear_color_r = EngineUtils::GetFloat(render_cfg, "clear_color_r", 255.0f);
		clear_color_g = EngineUtils::GetFloat(render_cfg, "clear_color_g", 255.0f);
		clear_color_b = EngineUtils::GetFloat(render_cfg, "clear_color_b", 255.0f);
		target_x_resolution = EngineUtils::GetInt(render_cfg, "target_x_resolution", 640);
		target_y_resolution = EngineUtils::GetInt(render_cfg, "target_y_resolution", 360);

		zoom_factor = EngineUtils::GetFloat(render_cfg, "zoom_factor", 1.0f);
		PIXELS_PER_METER = EngineUtils::GetInt(render_cfg, "pixels_per_meter", DEFAULT_PIXELS_PER_METER);

		resolution_scale_multiplier.x = ((float)x_resolution) / target_x_resolution;
		resolution_scale_multiplier.y = ((float)y_resolution) / target_y_resolution;

		clear_color_r /= 255.0f;
		clear_color_g /= 255.0f;
		clear_color_b /= 255.0f;

		// image rendering
		mono_add_internal_call("Scripts.Image::Draw", &Renderer::Image_Draw_Wrapper);
		mono_add_internal_call("Scripts.Image::DrawEx_Internal", &Renderer::Image_DrawEx_Fast);

		// UI rendering
		mono_add_internal_call("Scripts.Image::DrawUI", &Renderer::Image_DrawUI_Wrapper);
		mono_add_internal_call("Scripts.Image::DrawUIEx", &Renderer::Image_DrawUIEx_Wrapper);

		// singular pixel rendering
		mono_add_internal_call("Scripts.Image::DrawPixel", &Renderer::Image_DrawPixel);

		// camera
		mono_add_internal_call("Scripts.Camera::SetPosition", &Renderer::Camera_SetPosition);
		mono_add_internal_call("Scripts.Camera::GetPositionX", &Renderer::Camera_GetPositionX);
		mono_add_internal_call("Scripts.Camera::GetPositionY", &Renderer::Camera_GetPositionY);
		mono_add_internal_call("Scripts.Camera::SetZoom", &Renderer::Camera_SetZoom);
		mono_add_internal_call("Scripts.Camera::GetZoom", &Renderer::Camera_GetZoom);

		// text
		mono_add_internal_call("Scripts.Text::Draw", &Renderer::Text_Draw_Wrapper);
		// maybe dont need? dunno, will see
		// cam_offset_x = EngineUtils::GetFloat(render_cfg, "cam_offset_x", 0.0f);
		zoom_factor = EngineUtils::GetFloat(render_cfg, "zoom_factor", 1.0f);
		PIXELS_PER_METER = EngineUtils::GetInt(render_cfg, "pixels_per_meter", DEFAULT_PIXELS_PER_METER);
		// cam_ease_factor = EngineUtils::GetFloat(render_cfg, "cam_ease_factor", 1.0f);
		curr_cam_position = { 0.0f, 0.0f };

		if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
			std::cout << "SDL video subsystem could not initialize! SDL_Error: " << SDL_GetError() << "\n";
			exit(0);
		}

		Renderer::window = SDL_CreateWindow(
			game_title.c_str(),
			x_resolution,
			y_resolution,
			NULL
		);

		SDL_PropertiesID props = SDL_CreateProperties();
		SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, Renderer::window);
		SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER, SDL_COLORSPACE_SRGB_LINEAR);
		Renderer::renderer = SDL_CreateRendererWithProperties(props);
		SDL_DestroyProperties(props);

		if (!Renderer::renderer) {
			std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
			exit(0);
		}
		SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_PIXELART);
		SDL_SetRenderLogicalPresentation(renderer, x_resolution, y_resolution, SDL_LOGICAL_PRESENTATION_LETTERBOX);

		// initialize the command buffers with a bit of size
		draw_command_buffer.resize(8);
		draw_ui_command_buffer.resize(8);
		draw_text_command_buffer.resize(8);
		draw_pixel_command_buffer.resize(8);

		ImageDB::Initialize(Renderer::GetSDLRenderer());
		TextDB::Initialize();
		ParticleSystem::Initialize();
	}
}

void Renderer::Shutdown()
{
	if (renderer) { SDL_DestroyRenderer(renderer); }
	if (window) { SDL_DestroyWindow(window); }
}

void Renderer::ClearRenderer()
{
	SDL_SetRenderDrawColorFloat(renderer, clear_color_r, clear_color_g, clear_color_b, 1.0f);
	SDL_RenderClear(renderer);
}

void Renderer::ProcessCommandBuffer() {
	screen_size_constant_x = x_resolution * 0.5f * (1.0f / zoom_factor);
	screen_size_constant_y = y_resolution * 0.5f * (1.0f / zoom_factor);
	// Draw and DrawEx
	
	std::sort(draw_command_buffer.begin(), draw_command_buffer.begin() + num_draw_commands);
	SDL_SetRenderScale(renderer, zoom_factor, zoom_factor);
	for (size_t cmd_idx = 0; cmd_idx < num_draw_commands; ++cmd_idx) {
		DrawImageElement(draw_command_buffer[cmd_idx]);
	}
	SDL_SetRenderScale(renderer, 1, 1);

	// DrawUI and DrawUIEx
	
	std::stable_sort(draw_ui_command_buffer.begin(), draw_ui_command_buffer.begin() + num_draw_ui_commands);
	for (size_t cmd_idx = 0; cmd_idx < num_draw_ui_commands; ++cmd_idx) {
		DrawUIElement(draw_ui_command_buffer[cmd_idx]);
	}

	// Text
	
	for (size_t cmd_idx = 0; cmd_idx < num_draw_text_commands; ++cmd_idx) {
		DrawTextElement(draw_text_command_buffer[cmd_idx]);
	}

	// Pixels
	if (num_draw_commands > 0) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		for (size_t cmd_idx = 0; cmd_idx < num_draw_pixel_commands; ++cmd_idx) {
			DrawPixelElement(draw_pixel_command_buffer[cmd_idx]);
		}
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}

	// reset command buffers
	num_draw_commands = 0;
	num_draw_ui_commands = 0;
	num_draw_text_commands = 0;
	num_draw_pixel_commands = 0;
}

void Renderer::Present()
{
	SDL_RenderPresent(renderer);
}

void Renderer::DrawImageElement(const RenderCommand::DrawCommand& cmd)
{
	static SDL_FColor current_mod = { 1.0f, 1.0f, 1.0f, 1.0f };

	fvec2 dims = ImageDB::image_dimensions[cmd.image_idx];
	SDL_FRect& src_rect = ImageDB::image_src_rects[cmd.image_idx];

	fvec2 final_rendering_position = (fvec2(cmd.x, cmd.y) - Renderer::curr_cam_position)*PIXELS_PER_METER*resolution_scale_multiplier;
	fvec2 scale_mod = fvec2{ std::abs(cmd.scale_x) , std::abs(cmd.scale_y) }*resolution_scale_multiplier;

	int flip_mode = SDL_FLIP_NONE;
	if (cmd.scale_x < 0)
		flip_mode |= SDL_FLIP_HORIZONTAL;
	if (cmd.scale_y < 0)
		flip_mode |= SDL_FLIP_VERTICAL;

	fvec2 new_dims = dims * scale_mod;
	SDL_FPoint pivot_point = { cmd.pivot_x * new_dims.x, cmd.pivot_y * new_dims.y };

	if (cmd.color.r != current_mod.r || cmd.color.g != current_mod.g || cmd.color.b != current_mod.b || cmd.color.a != current_mod.a) {
		SDL_SetTextureColorModFloat(ImageDB::image_atlas, cmd.color.r, cmd.color.g, cmd.color.b);
		SDL_SetTextureAlphaModFloat(ImageDB::image_atlas, cmd.color.a);
		current_mod = cmd.color;
	}

	SDL_FRect dest_rect = 
						{
							(final_rendering_position.x + screen_size_constant_x - pivot_point.x),
							(final_rendering_position.y + screen_size_constant_y - pivot_point.y),
							new_dims.x,
							new_dims.y
						};

	SDL_RenderTextureRotated(renderer, ImageDB::image_atlas, &src_rect, &dest_rect, (double)cmd.rotation_degrees, &pivot_point, static_cast<SDL_FlipMode>(flip_mode));
}

void Renderer::DrawUIElement(const RenderCommand::DrawUICommand& cmd)
{
	uint32_t texture_idx = ImageDB::GetTextureIdx(cmd.image_name);
	//SDL_Texture* texture = ImageDB::GetTexture(texture_idx);
	//fvec2 dims = ImageDB::GetDimensions(texture_idx);

	/*SDL_FRect texture_rect{ cmd.x, cmd.y, dims.x, dims.y };

	SDL_SetTextureColorModFloat(texture, cmd.color.r, cmd.color.g, cmd.color.b);
	SDL_SetTextureAlphaModFloat(texture, cmd.color.a);

	SDL_RenderTexture(renderer, texture, nullptr, &texture_rect);

	SDL_SetTextureColorModFloat(texture, 1.0f, 1.0f, 1.0f);
	SDL_SetTextureAlphaModFloat(texture, 1.0f);*/
}
 
void Renderer::DrawTextElement(const RenderCommand::DrawTextCommand& cmd)
{
	if (cmd.str_content.empty()) return;

	TTF_Font* font_to_use = TextDB::GetFont(cmd.font_name, static_cast<size_t>(cmd.font_size));
	SDL_Surface* text_surface = TTF_RenderText_Solid(font_to_use, cmd.str_content.c_str(), cmd.str_content.length(), cmd.color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	float w = 0, h = 0;
	SDL_GetTextureSize(texture, &w, &h);
	SDL_FRect pos_rect{ cmd.x, cmd.y, w, h };

	SDL_RenderTexture(renderer, texture, nullptr, &pos_rect);
	
	// destroy stuff
	SDL_DestroyTexture(texture);
	SDL_DestroySurface(text_surface);
}

void Renderer::DrawPixelElement(const RenderCommand::DrawPixelCommand& cmd)
{
	SDL_SetRenderDrawColorFloat(renderer, cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
	SDL_RenderPoint(renderer, cmd.x, cmd.y);
}

void Renderer::Camera_SetPosition(float x, float y)
{
	curr_cam_position.x = x;
	curr_cam_position.y = y;
}

float Renderer::Camera_GetPositionX()
{
	return curr_cam_position.x;
}

float Renderer::Camera_GetPositionY()
{
	return curr_cam_position.y;
}

void Renderer::Camera_SetZoom(float zf)
{
	Renderer::zoom_factor = zf;
}

float Renderer::Camera_GetZoom()
{
	return zoom_factor;
}

void Renderer::Image_Draw(std::string& image_name, float x, float y)
{
	if (num_draw_commands >= draw_command_buffer.size())
		draw_command_buffer.resize(draw_command_buffer.size() * 2);

	RenderCommand::DrawCommand& cmd = draw_command_buffer[num_draw_commands];

	cmd.image_idx = ImageDB::GetTextureIdx(image_name);
	cmd.x = x;
	cmd.y = y;
	cmd.rotation_degrees = 0.0f;
	cmd.scale_x = 1.0f;
	cmd.scale_y = 1.0f;
	cmd.pivot_x = 0.5f;
	cmd.pivot_y = 0.5f;
	cmd.sorting_order = 0.0f;
	cmd.color.r = 1.0f;
	cmd.color.g = 1.0f;
	cmd.color.b = 1.0f;
	cmd.color.a = 1.0f;

	num_draw_commands++;
}

void Renderer::Image_Draw_Wrapper(MonoString* m_image_name, float x, float y)
{
	char* cstr = mono_string_to_utf8(m_image_name);
	std::string cpp_str(cstr);
	mono_free(cstr);
	Renderer::Image_Draw(cpp_str, x, y);
}

void Renderer::Image_DrawEx(std::string& image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, int sorting_order)
{
	if (num_draw_commands >= draw_command_buffer.size())
		draw_command_buffer.resize(draw_command_buffer.size() * 2);

	RenderCommand::DrawCommand& cmd = draw_command_buffer[num_draw_commands];

	cmd.image_idx = ImageDB::GetTextureIdx(image_name);
	cmd.x = x;
	cmd.y = y;
	cmd.rotation_degrees = rotation_degrees;
	cmd.scale_x = scale_x;
	cmd.scale_y = scale_y;
	cmd.pivot_x = pivot_x;
	cmd.pivot_y = pivot_y;
	cmd.sorting_order = (int32_t)sorting_order;
	cmd.color.r = r / 255.0f;
	cmd.color.g = g / 255.0f;
	cmd.color.b = b / 255.0f;
	cmd.color.a = a / 255.0f;

	num_draw_commands++;
}

void Renderer::Image_DrawEx_Wrapper(MonoString* m_image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, int sorting_order)
{
	char* cstr = mono_string_to_utf8(m_image_name);
	std::string cpp_str(cstr);
	mono_free(cstr);
	Renderer::Image_DrawEx(cpp_str, x, y, rotation_degrees, scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order);
}


void Renderer::Image_DrawEx_Fast(int image_idx, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, int sorting_order)
{
	if (num_draw_commands >= draw_command_buffer.size())
		draw_command_buffer.resize(draw_command_buffer.size() * 2);

	RenderCommand::DrawCommand& cmd = draw_command_buffer[num_draw_commands];

	cmd.image_idx = image_idx;
	cmd.x = x;
	cmd.y = y;
	cmd.rotation_degrees = rotation_degrees;
	cmd.scale_x = scale_x;
	cmd.scale_y = scale_y;
	cmd.pivot_x = pivot_x;
	cmd.pivot_y = pivot_y;
	cmd.sorting_order = (int32_t)sorting_order;
	cmd.color.r = r / 255.0f;
	cmd.color.g = g / 255.0f;
	cmd.color.b = b / 255.0f;
	cmd.color.a = a / 255.0f;

	num_draw_commands++;
}

void Renderer::Image_DrawUI(std::string& image_name, float x, float y)
{
	if (num_draw_ui_commands >= draw_ui_command_buffer.size())
		draw_ui_command_buffer.resize(draw_ui_command_buffer.size() * 2);

	RenderCommand::DrawUICommand& cmd = draw_ui_command_buffer[num_draw_ui_commands];

	cmd.image_name = image_name;
	cmd.sorting_order = 0;
	cmd.x = x;
	cmd.y = y;
	cmd.color.r = 1.0f;
	cmd.color.g = 1.0f;
	cmd.color.b = 1.0f;
	cmd.color.a = 1.0f;

	num_draw_ui_commands++;
}

void Renderer::Image_DrawUI_Wrapper(MonoString* m_image_name, float x, float y)
{
	char* cstr = mono_string_to_utf8(m_image_name);
	std::string cpp_str(cstr);
	mono_free(cstr);
	Renderer::Image_DrawUI(cpp_str, x, y);
}

void Renderer::Image_DrawUIEx(std::string& image_name, float x, float y, float r, float g, float b, float a, int sorting_order)
{
	if (num_draw_ui_commands >= draw_ui_command_buffer.size())
		draw_ui_command_buffer.resize(draw_ui_command_buffer.size() * 2);

	RenderCommand::DrawUICommand& cmd = draw_ui_command_buffer[num_draw_ui_commands];

	cmd.image_name = image_name;
	cmd.sorting_order = (int32_t)sorting_order;
	cmd.x = x;
	cmd.y = y;
	cmd.color.r = r / 255.0f;
	cmd.color.g = g / 255.0f;
	cmd.color.b = b / 255.0f;
	cmd.color.a = a / 255.0f;

	num_draw_ui_commands++;
}

void Renderer::Image_DrawUIEx_Wrapper(MonoString* m_image_name, float x, float y, float r, float g, float b, float a, int sorting_order)
{
	char* cstr = mono_string_to_utf8(m_image_name);
	std::string cpp_str(cstr);
	mono_free(cstr);
	Renderer::Image_DrawUIEx(cpp_str, x, y, r, g, b, a, sorting_order);
}

void Renderer::Text_Draw(std::string& str_content, float x, float y, std::string& font_name, int font_size, int r, int g, int b, int a)
{
	if (num_draw_text_commands >= draw_text_command_buffer.size())
		draw_text_command_buffer.resize(draw_text_command_buffer.size() * 2);

	RenderCommand::DrawTextCommand& cmd = draw_text_command_buffer[num_draw_text_commands];

	cmd.str_content = str_content;
	cmd.font_name = font_name;
	cmd.x = x;
	cmd.y = y;
	cmd.font_size = (int)font_size;
	cmd.color.r = r;
	cmd.color.g = g;
	cmd.color.b = b;
	cmd.color.a = a;

	num_draw_text_commands++;
}

void Renderer::Text_Draw_Wrapper(MonoString* str_content, float x, float y, MonoString* font_name, int font_size, int r, int g, int b, int a)
{
	char* cstr_1 = mono_string_to_utf8(str_content);
	char* cstr_2 = mono_string_to_utf8(font_name);
	std::string cpp_str_1(cstr_1);
	std::string cpp_str_2(cstr_2);
	mono_free(cstr_1);
	mono_free(cstr_2);

	Renderer::Text_Draw(cpp_str_1, x, y, cpp_str_2, font_size, r, g, b, a);
}

void Renderer::Image_DrawPixel(int x, int y, float r, float g, float b, float a)
{
	if (num_draw_pixel_commands >= draw_pixel_command_buffer.size())
		draw_pixel_command_buffer.resize(draw_pixel_command_buffer.size() * 2);

	RenderCommand::DrawPixelCommand& cmd = draw_pixel_command_buffer[num_draw_pixel_commands];

	cmd.x = x;
	cmd.y = y;
	cmd.color.r = r / 255.0f;
	cmd.color.g = g / 255.0f;
	cmd.color.b = b / 255.0f;
	cmd.color.a = a / 255.0f;

	num_draw_pixel_commands++;
}