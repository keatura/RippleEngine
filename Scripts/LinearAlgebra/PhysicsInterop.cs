using Engine.LinearAlgebra;
using System;
using System.Collections.Generic;
using System.Diagnostics.SymbolStore;
using System.Runtime.InteropServices;
using System.Text;
using Scripts;

namespace Engine.Physics
{
    public static class PhysicsInterop
    {
        // physics constants
        public static float b2_pi { get; private set; } = 3.14159265359f;
        public static float BASE_X_POS { get; private set; } = 0.0f;
        public static float BASE_Y_POS { get; private set; } = 0.0f;
        public static bool BASE_PRECISION { get; private set; } = true;
        public static float BASE_GRAV_SCALE { get; private set; } = 1.0f;
        public static float BASE_DENSITY { get; private set; } = 1.0f;
        public static float BASE_ANGULAR_FRICTION { get; private set; } = 0.3f;
        public static float BASE_ROTATION { get; private set; } = 0.0f;
        public static bool BASE_HAS_COLLIDER { get; private set; } = true;
        public static bool BASE_HAS_TRIGGER { get; private set; } = true;

        public static float BASE_TRIGGER_WIDTH { get; private set; } = 1.0f;
        public static float BASE_TRIGGER_HEIGHT { get; private set; } = 1.0f;
        public static float BASE_TRIGGER_RADIUS { get; private set; } = 0.5f;


        public static float BASE_WIDTH { get; private set; } = 1.0f;
        public static float BASE_HEIGHT { get; private set; } = 1.0f;
        public static float BASE_RADIUS { get; private set; } = 0.5f;
        public static float BASE_FRICTION { get; private set; } = 0.3f;
        public static float BASE_BOUNCINESS { get; private set; } = 0.3f;
        public static float RIGHT_DEGREES_CONSTANT { get; private set; } = (b2_pi / 2.0f);
        public static float RADIANS_TO_DEGREES_CONSTANT { get; private set; } = (180.0f / b2_pi);
        public static float DEGREES_TO_RADIANS_CONSTANT { get; private set; } = (b2_pi / 180.0f);

        private const string DllName = "game_engine.exe";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateBody(
            string body_type,
            string collider_type,
            string trigger_type,
            float grav_scale,
            float density,
            float angular_friction,
            float rotation,
            float width,
            float height,
            float radius,
            float friction,
            float bounciness,
            float trigger_width,
            float trigger_height,
            float trigger_radius,
            float x,
            float y,
            bool precise,
            bool has_collider,
            bool has_trigger);

        // position
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern float GetX(IntPtr body_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern float GetY(IntPtr body_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetX(IntPtr body_ptr, float x);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetY(IntPtr body_ptr, float y);

        // members
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AddForce(IntPtr body_ptr, float force_x, float force_y);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetVelocity(IntPtr body_ptr, float vel_x, float vel_y);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetPosition(IntPtr body_ptr, float pos_x, float pos_y);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetRotation(IntPtr body_ptr, float degrees_clockwise);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetAngularVelocity(IntPtr body_ptr, float degrees_clockwise);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetGravityScale(IntPtr body_ptr, float gravity_scale);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetUpDirection(IntPtr body_ptr, float up_direction_x, float up_direction_y);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetRightDirection(IntPtr body_ptr, float right_direction_x, float right_direction_y);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern float GetAngularVelocity(IntPtr body_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern float GetGravityScale(IntPtr body_ptr);
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern float GetRotation(IntPtr body_ptr);



        // vector stuff
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetVelocity_Inner(IntPtr body_ptr, out float vel_x, out float vel_y);
        public static Vector2 GetVelocity(IntPtr body_ptr)
        {
            GetVelocity_Inner(body_ptr, out float vel_x, out float vel_y);
            return new Vector2(vel_x, vel_y);
        }

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetUpDirection_Inner(IntPtr body_ptr, out float up_x, out float up_y);
        public static Vector2 GetUpDirection(IntPtr body_ptr)
        {
            GetUpDirection_Inner(body_ptr, out float up_x, out float up_y);
            return new Vector2(up_x, up_y);
        }

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetRightDirection_Inner(IntPtr body_ptr, out float right_x, out float right_y);
        public static Vector2 GetRightDirection(IntPtr body_ptr)
        {
            GetRightDirection_Inner(body_ptr, out float right_x, out float right_y);
            return new Vector2(right_x, right_y);
        }

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetPosition_Inner(IntPtr body_ptr, out float position_x, out float position_y);
        public static Vector2 GetPosition(IntPtr body_ptr) {
            GetPosition_Inner(body_ptr, out float position_x, out float position_y);
            return new Vector2 (position_x, position_y);
        }

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyBody(IntPtr body_ptr);
    }
}
