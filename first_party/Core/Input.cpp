#include "Core/Input.h"
#include "Core/Engine.h"

#include "mono/metadata/object.h"
#include "mono/metadata/appdomain.h"

void Input::Initialize()
{
	if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD)) {
		std::cout << "SDL video subsystem could not initialize! SDL_Error: " << SDL_GetError() << "\n";
		exit(0);
	}

	keyboard_states.resize(SDL_SCANCODE_COUNT, INPUT_STATE_UP);
	just_became_down_scancodes.reserve(SDL_SCANCODE_COUNT);
	just_became_up_scancodes.reserve(SDL_SCANCODE_COUNT);


	mouse_states.resize(32, INPUT_STATE_UP); // probably dont need any more
	just_became_down_mouse_buttons.reserve(32);
	just_became_up_mouse_buttons.reserve(32);

	gamepad_states.resize(SDL_GAMEPAD_BUTTON_COUNT, INPUT_STATE_UP);
	gamepad_axes.resize(SDL_GAMEPAD_AXIS_COUNT, 0.0f);
	just_became_down_gamepad_buttons.reserve(SDL_GAMEPAD_BUTTON_COUNT);
	just_became_up_gamepad_buttons.reserve(SDL_GAMEPAD_BUTTON_COUNT);

	curr_mouse_scroll_delta = 0.0f;

	just_became_up_scancodes.reserve(512);
	just_became_down_scancodes.reserve(512);

	// keyboard input
	mono_add_internal_call("Scripts.Input::Cpp_GetKey", &Input::GetKey);
	mono_add_internal_call("Scripts.Input::Cpp_GetKeyDown", &Input::GetKeyDown);
	mono_add_internal_call("Scripts.Input::Cpp_GetKeyUp", &Input::GetKeyUp);

	// mouse button input
	mono_add_internal_call("Scripts.Input::Cpp_GetMouseButton", &Input::GetMouseButton);
	mono_add_internal_call("Scripts.Input::Cpp_GetMouseButtonDown", &Input::GetMouseButtonDown);
	mono_add_internal_call("Scripts.Input::Cpp_GetMouseButtonUp", &Input::GetMouseButtonUp);
	mono_add_internal_call("Scripts.Input::GetMousePositionX", &Input::GetMousePositionX);
	mono_add_internal_call("Scripts.Input::GetMousePositionY", &Input::GetMousePositionY);

	// mouse scroll input
	mono_add_internal_call("Scripts.Input::Cpp_GetMouseScrollDelta", &Input::GetMouseScrollDelta);

	// mouse cursor
	mono_add_internal_call("Scripts.Input::HideMouseCursor", &Input::HideMouseCursor);
	mono_add_internal_call("Scripts.Input::ShowMouseCursor", &Input::ShowMouseCursor);
	mono_add_internal_call("Scripts.Input::IsMouseCursorHidden", &Input::IsMouseCursorHidden);

	// gamepad
	mono_add_internal_call("Scripts.Input::Cpp_GetGamepadButton", &Input::GetGamepadButton);
	mono_add_internal_call("Scripts.Input::Cpp_GetGamepadButtonDown", &Input::GetGamepadButtonDown);
	mono_add_internal_call("Scripts.Input::Cpp_GetGamepadButtonUp", &Input::GetGamepadButtonUp);
	mono_add_internal_call("Scripts.Input::Cpp_GetGamepadAxis", &Input::GetGamepadAxis);
}

void Input::ProcessEvents()
{
	SDL_Event next_event;
	while (SDL_PollEvent(&next_event)) {
		switch (next_event.type) {
		case SDL_EVENT_KEY_DOWN:
		{
			SDL_Scancode curr_scancode = next_event.key.scancode;
			keyboard_states[curr_scancode] = INPUT_STATE_JUST_BECAME_DOWN;
			just_became_down_scancodes.push_back(curr_scancode);
		}
		break;
		case SDL_EVENT_KEY_UP:
		{
			SDL_Scancode curr_scancode = next_event.key.scancode;
			keyboard_states[curr_scancode] = INPUT_STATE_JUST_BECAME_UP;
			just_became_up_scancodes.push_back(curr_scancode);
		}
		break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			Uint8 curr_mouse_button = next_event.button.button;
			mouse_states[curr_mouse_button] = INPUT_STATE_JUST_BECAME_DOWN;
			just_became_down_mouse_buttons.push_back(curr_mouse_button);
		}
		break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
		{
			Uint8 curr_mouse_button = next_event.button.button;
			mouse_states[curr_mouse_button] = INPUT_STATE_JUST_BECAME_UP;
			just_became_up_mouse_buttons.push_back(curr_mouse_button);
		}
		break;
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		{
			Uint8 curr_gamepad_button = next_event.gbutton.button;
			gamepad_states[curr_gamepad_button] = INPUT_STATE_JUST_BECAME_DOWN;
			just_became_down_gamepad_buttons.push_back(curr_gamepad_button);
		}
		break;
		case SDL_EVENT_GAMEPAD_BUTTON_UP:
		{
			Uint8 curr_gamepad_button = next_event.gbutton.button;
			gamepad_states[curr_gamepad_button] = INPUT_STATE_JUST_BECAME_UP;
			just_became_up_gamepad_buttons.push_back(curr_gamepad_button);
		}
		break;
		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
		{
			Uint8 curr_axis = next_event.gaxis.axis;
			float normalized_value = 0.0f;

			if (SDL_abs(next_event.gaxis.value) > GAMEPAD_AXIS_DEADZONE) // only accept values greater than the deadzone
				normalized_value = next_event.gaxis.value / 32767.0f;
			gamepad_axes[curr_axis] = normalized_value;
		}
			break;
		case SDL_EVENT_QUIT:
			Engine::engine_running = false;
			break;
		case SDL_EVENT_MOUSE_MOTION:
			mouse_position.x = (float)next_event.motion.x;
			mouse_position.y = (float)next_event.motion.y;
			break;
		case SDL_EVENT_MOUSE_WHEEL:
			curr_mouse_scroll_delta = next_event.wheel.y;
			break;
		case SDL_EVENT_GAMEPAD_ADDED:
			gamepad_exists = true;
			gamepad = SDL_OpenGamepad(next_event.gdevice.which);
			if (gamepad) {
				SDL_Log("Opened controller: %s", SDL_GetGamepadName(gamepad));
			}
			break;
		case SDL_EVENT_GAMEPAD_REMOVED:
			gamepad_exists = false;
			CloseGamepad();
			break;
		case SDL_EVENT_KEYBOARD_ADDED:
			keyboard_exists = true;
			break;
		case SDL_EVENT_KEYBOARD_REMOVED:
			keyboard_exists = false;
			break;
		case SDL_EVENT_MOUSE_ADDED:
			mouse_exists = true;
			break;
		case SDL_EVENT_MOUSE_REMOVED:
			mouse_exists = false;
			break;
		default:
			break;
		}
	}
}

void Input::LateUpdate()
{
	curr_mouse_scroll_delta = 0.0f;

	for (const SDL_Scancode scancode : just_became_down_scancodes) {
		keyboard_states[scancode] = INPUT_STATE_DOWN;
	}
	for (const SDL_Scancode scancode : just_became_up_scancodes) {
		keyboard_states[scancode] = INPUT_STATE_UP;
	}
	for (const Uint8 mouse_code : just_became_down_mouse_buttons) {
		mouse_states[mouse_code] = INPUT_STATE_DOWN;
	}
	for (const Uint8 mouse_code : just_became_up_mouse_buttons) {
		mouse_states[mouse_code] = INPUT_STATE_UP;
	}
	for (const Uint8 gamepad_code : just_became_down_gamepad_buttons) {
		gamepad_states[gamepad_code] = INPUT_STATE_DOWN;
	}
	for (const Uint8 gamepad_code : just_became_up_gamepad_buttons) {
		gamepad_states[gamepad_code] = INPUT_STATE_UP;
	}

	just_became_down_scancodes.clear();
	just_became_up_scancodes.clear();
	just_became_down_mouse_buttons.clear();
	just_became_up_mouse_buttons.clear();
}

bool Input::GetKey(int keycode) // true if down/just pressed down on this frame
{
	return keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN || keyboard_states[keycode] == INPUT_STATE_DOWN;
}

bool Input::GetKeyDown(int keycode) // true if pressed on this frame
{
	return keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetKeyUp(int keycode) // true if just released on on this frame
{
	return keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_UP;
}

bool Input::GetMouseButton(int button)
{
	return mouse_states[button] == INPUT_STATE_JUST_BECAME_DOWN || mouse_states[button] == INPUT_STATE_DOWN;
}

bool Input::GetMouseButtonDown(int button)
{
	return mouse_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetMouseButtonUp(int button)
{
	return mouse_states[button] == INPUT_STATE_JUST_BECAME_UP;
}

bool Input::GetGamepadButton(int button)
{
	return gamepad_states[button] == INPUT_STATE_JUST_BECAME_DOWN || gamepad_states[button] == INPUT_STATE_DOWN;
}

bool Input::GetGamepadButtonDown(int button)
{
	return gamepad_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetGamepadButtonUp(int button)
{
	return gamepad_states[button] == INPUT_STATE_JUST_BECAME_UP;
}

float Input::GetGamepadAxis(int axis)
{
	return gamepad_axes[axis];
}

void Input::HideMouseCursor()
{
	SDL_HideCursor();
}

void Input::ShowMouseCursor()
{
	SDL_ShowCursor();
}

bool Input::IsMouseCursorHidden()
{
	return !SDL_CursorVisible();
}

float Input::GetMouseScrollDelta()
{
	return curr_mouse_scroll_delta;
}

float Input::GetMousePositionX() {
	return mouse_position.x;
}

float Input::GetMousePositionY() {
	return mouse_position.y;
}

void Input::CloseGamepad()
{
	SDL_Log("Closed controller: %s", SDL_GetGamepadName(gamepad));
	SDL_CloseGamepad(gamepad);
	gamepad = nullptr;
}

void Input::Shutdown() {
	CloseGamepad();
}