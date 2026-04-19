#ifndef INPUT_H
#define INPUT_H

#include "SDL3/SDL.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>

#include "FastVector.h"

enum INPUT_STATE : uint8_t { 
	INPUT_STATE_UP, 
	INPUT_STATE_JUST_BECAME_DOWN, 
	INPUT_STATE_DOWN, 
	INPUT_STATE_JUST_BECAME_UP 
};

inline constexpr float GAMEPAD_AXIS_DEADZONE = 1638; // roughly 5% deadzone

class Input
{
public:
	static void Initialize();
	static void Shutdown();
	static void ProcessEvents();
	static void LateUpdate();

	static bool GetKey(int keycode);
	static bool GetKeyDown(int keycode);
	static bool GetKeyUp(int keycode);

	static bool GetMouseButton(int button);
	static bool GetMouseButtonDown(int button);
	static bool GetMouseButtonUp(int button);

	static bool GetGamepadButton(int button);
	static bool GetGamepadButtonDown(int button);
	static bool GetGamepadButtonUp(int button);
	static float GetGamepadAxis(int axis);

	static void HideMouseCursor();
	static void ShowMouseCursor();
	static bool IsMouseCursorHidden();

	static float GetMouseScrollDelta();

	static float GetMousePositionX();
	static float GetMousePositionY();
	 
private:
	static inline bool gamepad_exists = false;
	static inline bool keyboard_exists = false;
	static inline bool mouse_exists = false;
	static void CloseGamepad();

	// Keyboard input
	static inline std::vector<INPUT_STATE> keyboard_states;
	static inline std::vector<SDL_Scancode> just_became_down_scancodes;
	static inline std::vector<SDL_Scancode> just_became_up_scancodes;

	// Mouse input
	static inline std::vector<INPUT_STATE> mouse_states;
	static inline std::vector<Uint8> just_became_down_mouse_buttons;
	static inline std::vector<Uint8> just_became_up_mouse_buttons;
	static inline fvec2 mouse_position{ 0.0f,0.0f };
	static inline float curr_mouse_scroll_delta = 0.0f;

	// Gamepad input
	static inline std::vector<INPUT_STATE> gamepad_states;
	static inline std::vector<Uint8> just_became_down_gamepad_buttons;
	static inline std::vector<Uint8> just_became_up_gamepad_buttons;
	static inline std::vector<float> gamepad_axes; // normalized to -1.0, 1.0
	static inline SDL_Gamepad* gamepad = nullptr;

 };

#endif