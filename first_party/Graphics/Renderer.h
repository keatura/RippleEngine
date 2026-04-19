#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <unordered_map>

#include "SDL3/SDL.h" // TODO

#include "RenderCommands.h"
#include "Math/FastVector.h"

typedef struct _MonoString MonoString;

class Renderer
{
public:
	static void Initialize(const std::string& game_title);
	static void Shutdown();
	static void ClearRenderer();
	static void ProcessCommandBuffer();
	static void Present();
	// void UpdateCameraCenter();

	static SDL_Renderer* GetSDLRenderer() { return renderer; }
	static inline bool x_scale_actor_flipping_on_movement = false;

	static void Image_Draw(std::string& image_name, float x, float y);
	static void Image_Draw_Wrapper(MonoString* m_image_name, float x, float y);

	static void Image_DrawEx(std::string& image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, int sorting_order);
	static void Image_DrawEx_Wrapper(MonoString* m_image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, int sorting_order);

	static void Image_DrawEx_Fast(int image_idx, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, int sorting_order);

	static void Image_DrawUI(std::string& image_name, float x, float y);
	static void Image_DrawUI_Wrapper(MonoString* m_image_name, float x, float y);

	static void Image_DrawUIEx(std::string& image_name, float x, float y, float r, float g, float b, float a, int sorting_order);
	static void Image_DrawUIEx_Wrapper(MonoString* m_image_name, float x, float y, float r, float g, float b, float a, int sorting_order);
	
	static void Text_Draw(std::string& str_content, float x, float y, std::string& font_name, int font_size, int r, int g, int b, int a);
	static void Text_Draw_Wrapper(MonoString* str_content, float x, float y, MonoString* font_name, int font_size, int r, int g, int b, int a);

	static void Image_DrawPixel(int x, int y, float r, float g, float b, float a);

	static inline std::string game_title = "";
	static inline SDL_Window* window = nullptr;
	static inline SDL_Renderer* renderer = nullptr;

	static inline fvec2 base_cam_center{ 0.0f, 0.0f };
	static inline fvec2 curr_cam_position{ 0.0f, 0.0f };
	static inline float cam_offset_x = 0;
	static inline float cam_offset_y = 0;

	static inline float zoom_factor = 1.0f;
	static inline float cam_ease_factor = 1.0f;

	static inline int32_t target_x_resolution = 640;
	static inline int32_t target_y_resolution = 360;
	static inline int32_t x_resolution = 640;
	static inline int32_t y_resolution = 360;
	static inline fvec2 resolution_scale_multiplier = { 1.0f, 1.0f };

	static inline float clear_color_r = 255;
	static inline float clear_color_g = 255;
	static inline float clear_color_b = 255;

private:
	static inline float screen_size_constant_x = 0.0f;
	static inline float screen_size_constant_y = 0.0f;

	// beeg buffer
	// for draw commands
	static inline std::vector<RenderCommand::DrawCommand> draw_command_buffer;
	static inline std::vector<RenderCommand::DrawUICommand> draw_ui_command_buffer;
	static inline std::vector<RenderCommand::DrawTextCommand> draw_text_command_buffer;
	static inline std::vector<RenderCommand::DrawPixelCommand> draw_pixel_command_buffer;

	static inline size_t num_draw_commands = 0;
	static inline size_t num_draw_ui_commands = 0;
	static inline size_t num_draw_text_commands = 0;
	static inline size_t num_draw_pixel_commands = 0;

	static void DrawImageElement(const RenderCommand::DrawCommand& cmd);
	static void DrawUIElement(const RenderCommand::DrawUICommand& cmd);
	static void DrawTextElement(const RenderCommand::DrawTextCommand& cmd);
	static void DrawPixelElement(const RenderCommand::DrawPixelCommand& cmd);

	static inline constexpr int DEFAULT_PIXELS_PER_METER = 8;
	static inline int PIXELS_PER_METER = DEFAULT_PIXELS_PER_METER;

	static void Camera_SetPosition(float x, float y);
	static float Camera_GetPositionX();
	static float Camera_GetPositionY();
	static void Camera_SetZoom(float zf);
	static float Camera_GetZoom();
};

#endif