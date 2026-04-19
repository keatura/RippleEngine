#pragma once

#include <cstdint>

inline constexpr float DEFAULT_ORIGIN_X = 0.0f;
inline constexpr float DEFAULT_ORIGIN_Y = 0.0f;

inline constexpr size_t DEFAULT_FRAMES_BETWEEN_BURSTS = 1;
inline constexpr size_t DEFAULT_BURST_QUANTITY = 1; // burst quantity
inline constexpr size_t DEFAULT_DURATION_FRAMES = 300; // 0 means infinite duration

inline constexpr float DEFAULT_START_SCALE_MIN = 1.0f;
inline constexpr float DEFAULT_START_SCALE_MAX = 1.0f;

inline constexpr float DEFAULT_ROTATION_MIN = 0.0f;
inline constexpr float DEFAULT_ROTATION_MAX = 0.0f;

inline constexpr uint8_t DEFAULT_START_COLOR_R = 255;
inline constexpr uint8_t DEFAULT_START_COLOR_G = 255;
inline constexpr uint8_t DEFAULT_START_COLOR_B = 255;
inline constexpr uint8_t DEFAULT_START_COLOR_A = 255;

inline constexpr float DEFAULT_EMIT_RADIUS_MIN = 0.0f;
inline constexpr float DEFAULT_EMIT_RADIUS_MAX = 0.5f;

inline constexpr float DEFAULT_EMIT_ANGLE_MIN = 0.0f;
inline constexpr float DEFAULT_EMTI_ANGLE_MAX = 360.0f;

inline constexpr const char* DEFAULT_IMAGE = "";

inline constexpr int32_t DEFAULT_SORTING_ORDER = 9999;

inline constexpr float DEFAULT_START_SPEED_MIN = 0.0f;
inline constexpr float DEFAULT_START_SPEED_MAX = 0.0f;

inline constexpr float DEFAULT_ROTATION_SPEED_MIN = 0.0f;
inline constexpr float DEFAULT_ROTATION_SPEED_MAX = 0.0f;

inline constexpr float DEFAULT_GRAVITY_SCALE_X = 0.0f;
inline constexpr float DEFAULT_GRAVITY_SCALE_Y = 0.0f;

inline constexpr float DEFAULT_DRAG_FACTOR = 1.0f;

inline constexpr float DEFAULT_ANGULAR_DRAG_FACTOR = 1.0f;

inline constexpr float DEFAULT_END_SCALE = 1.0f;

inline constexpr uint8_t DEFAULT_END_COLOR_R = 255;
inline constexpr uint8_t DEFAULT_END_COLOR_G = 255;
inline constexpr uint8_t DEFAULT_END_COLOR_B = 255;
inline constexpr uint8_t DEFAULT_END_COLOR_A = 255;

inline constexpr float DEFAULT_PIVOT_POINT_X = 0.5f;
inline constexpr float DEFAULT_PIVOT_POINT_Y = 0.5f;