using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;
using Engine.LinearAlgebra;

namespace Scripts
{
    public class Input
    {
        // keyboard input
        // pretty much verbatim the SDL scancode list
        public static bool GetKey(InputCode key)
        {
            return Cpp_GetKey((int)key);
        }

        public static bool GetKeyDown(InputCode key)
        {
            return Cpp_GetKeyDown((int)key);
        }
        public static bool GetKeyUp(InputCode key)
        {
            return Cpp_GetKeyUp((int)key);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Cpp_GetKey(int keycode);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Cpp_GetKeyDown(int keycode);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Cpp_GetKeyUp(int keycode);

        // mouse button input
        public static bool GetMouseButton(MouseButton button)
        {
            return Cpp_GetMouseButton((int)button);
        }
        public static bool GetMouseButtonDown(MouseButton button)
        {
            return Cpp_GetMouseButtonDown((int)button);
        }
        public static bool GetMouseButtonUp(MouseButton button)
        {
            return Cpp_GetMouseButtonUp((int)button);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Cpp_GetMouseButton(int button_code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Cpp_GetMouseButtonDown(int button_code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Cpp_GetMouseButtonUp(int button_code);

        // mouse scroll input
        public static float GetMouseScrollDelta()
        {
            return Cpp_GetMouseScrollDelta();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Cpp_GetMouseScrollDelta();

        // mouse cursor
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void HideMouseCursor();
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void ShowMouseCursor();
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool IsMouseCursorHidden();

        // mouse position
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern float GetMousePositionX();
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern float GetMousePositionY();
        public static Vector2 GetMousePosition()
        {
            return new Vector2(GetMousePositionX(), GetMousePositionY());
        }

        // Gamepad stuff
        public static bool GetGamepadButton(GamepadButton button)
        {
            return Cpp_GetGamepadButton((int)button);
        }
        public static bool GetGamepadButtonDown(GamepadButton button)
        {
            return Cpp_GetGamepadButtonDown((int)button);
        }
        public static bool GetGamepadButtonUp(GamepadButton button)
        {
            return Cpp_GetGamepadButtonUp((int)button);
        }
        public static float GetGamepadAxis(GamepadAxis axis)
        {
            return Cpp_GetGamepadAxis((int)axis);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Cpp_GetGamepadButton(int button_code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Cpp_GetGamepadButtonDown(int button_code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Cpp_GetGamepadButtonUp(int button_code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Cpp_GetGamepadAxis(int axis_code);
    }
}
