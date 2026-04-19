#ifndef RENDERCOMMANDS_H
#define RENDERCOMMANDS_H

#include <string>
#include <cstdint>
#include "FastVector.h"
#include "SDL3/SDL.h"

namespace RenderCommand {
    // Draw text

    struct DrawTextCommand {
        std::string str_content = "";
        std::string font_name = "";
        float x = 0.0f;
        float y = 0.0f;
        int font_size = 0;
        SDL_Color color = {};
    };

    // Draw UI extended
    struct DrawUICommand {
        std::string image_name = "";
        int32_t sorting_order = 0;
        float x = 0.0f;
        float y = 0.0f;
        SDL_FColor color = {};

        bool operator<(const DrawUICommand& other) const {
            return sorting_order < other.sorting_order;
        }
    };

    // Draw extended
    struct DrawCommand{
        int image_idx = 0;
        float x = 0.0f;
        float y = 0.0f;
        float rotation_degrees = 0.0f;
        float scale_x = 0.0f;
        float scale_y = 0.0f;
        float pivot_x = 0.5f;
        float pivot_y = 0.5f;
        int32_t sorting_order = 0;
        SDL_FColor color = {};

        bool operator<(const DrawCommand& other) const {
            return sorting_order < other.sorting_order;
        }
    };

    struct SpriteDrawCommand {
        int image_id = 0;

        int layer = 0;
        int sorting_order = 0;

        float rotation_degrees = 0;

        float x = 0;
        float y = 0;
        float scale_x = 0;
        float scale_y = 0;
        float pivot_x = 0.5f;
        float pivot_y = 0.5f;
        SDL_FColor color = {};
        bool enabled = true;

        bool operator<(const SpriteDrawCommand& other) const {
            return sorting_order < other.sorting_order;
        }
    };

    // Draw pixel
    struct DrawPixelCommand {
        int32_t x = 0;
        int32_t y = 0;
        SDL_FColor color = {};
    };
}

#endif