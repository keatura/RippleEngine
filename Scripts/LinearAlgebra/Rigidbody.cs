using Engine.LinearAlgebra;
using Scripts.ECS;
using System;
using Scripts;
using Scripts.NativeComponents;

namespace Engine.Physics
{
    public struct Rigidbody : IComponent, IHasStart, IHasDestroy, IHasEarlyDataUpdate
    {

        public string body_type = "dynamic";
        public string collider_type = "box";
        public string trigger_type = "box";

        public float gravity_scale = PhysicsInterop.BASE_GRAV_SCALE;
        public float density = PhysicsInterop.BASE_DENSITY;
        public float angular_friction = PhysicsInterop.BASE_ANGULAR_FRICTION;
        public float rotation = PhysicsInterop.BASE_ROTATION;
        public float width = PhysicsInterop.BASE_WIDTH;
        public float height = PhysicsInterop.BASE_HEIGHT;
        public float radius = PhysicsInterop.BASE_RADIUS;
        public float friction = PhysicsInterop.BASE_FRICTION;
        public float bounciness = PhysicsInterop.BASE_BOUNCINESS;

        public float trigger_width = PhysicsInterop.BASE_TRIGGER_WIDTH;
        public float trigger_height = PhysicsInterop.BASE_TRIGGER_HEIGHT;
        public float trigger_radius = PhysicsInterop.BASE_TRIGGER_RADIUS;

        public bool precise = PhysicsInterop.BASE_PRECISION;
        public bool has_collider = PhysicsInterop.BASE_HAS_COLLIDER;
        public bool has_trigger = PhysicsInterop.BASE_HAS_TRIGGER;

        public IntPtr native_body_ptr = IntPtr.Zero;

        public Rigidbody() { }

        public void OnStart(int self_actor_id)
        {
            ref Position pos = ref Actor.GetComponent<Position>(self_actor_id);
            native_body_ptr = PhysicsInterop.CreateBody(
                body_type, collider_type, trigger_type,
                gravity_scale, density, angular_friction, 
                rotation, width, height, radius, friction, bounciness, 
                trigger_width, trigger_height, trigger_radius,
                pos.x, pos.y, precise, has_collider, has_trigger);
        }

        public void OnDestroy(int self_actor_id)
        {
            if (native_body_ptr == IntPtr.Zero) return;
            PhysicsInterop.DestroyBody(native_body_ptr);
            native_body_ptr = IntPtr.Zero;
        }

        public float GetX() {
            if (native_body_ptr == IntPtr.Zero) return 0.0f;
            return PhysicsInterop.GetX(native_body_ptr);
        }
        public float GetY() {
            if (native_body_ptr == IntPtr.Zero) return 0.0f;
            return PhysicsInterop.GetY(native_body_ptr);
        }
        public void SetX(float x) {
            if (native_body_ptr == IntPtr.Zero) return;
            PhysicsInterop.SetX(native_body_ptr, x);
        }
        public void SetY(float y) {
            if (native_body_ptr == IntPtr.Zero) return;
            PhysicsInterop.SetY(native_body_ptr, y);
        }

        public void AddForce(Vector2 force) {
            if (native_body_ptr == IntPtr.Zero) return;
            PhysicsInterop.AddForce(native_body_ptr, force.x, force.y);
        }

        public void SetVelocity(Vector2 new_velocty) {
            if (native_body_ptr == IntPtr.Zero) return;
            PhysicsInterop.SetVelocity(native_body_ptr, new_velocty.x, new_velocty.y);
        }
        public void SetPosition(Vector2 new_pos) {
            if (native_body_ptr == IntPtr.Zero) return;
            PhysicsInterop.SetPosition(native_body_ptr, new_pos.x, new_pos.y);
        }
        public void SetRotation(float degrees_clockwise) {
            if (native_body_ptr == IntPtr.Zero)
            {
                this.rotation = degrees_clockwise;
                return;
            }
            PhysicsInterop.SetRotation(native_body_ptr, degrees_clockwise);
        }
        public void SetAngularVelocity(float degrees_clockwise) {
            if (native_body_ptr == IntPtr.Zero) return;
            PhysicsInterop.SetAngularVelocity(native_body_ptr, degrees_clockwise);
        }
        public void SetGravityScale(float gravity_scale) {
            if (native_body_ptr == IntPtr.Zero)
            {
                this.gravity_scale = gravity_scale;
                return;
            }
            PhysicsInterop.SetGravityScale(native_body_ptr, gravity_scale);
        }
        public void SetUpDirection(Vector2 direction) {
            if (native_body_ptr == IntPtr.Zero) return;
            PhysicsInterop.SetUpDirection(native_body_ptr, direction.x, direction.y);
        }
        public void SetRightDirection(Vector2 direction) {
            if (native_body_ptr == IntPtr.Zero) return;
            PhysicsInterop.SetRightDirection(native_body_ptr, direction.x, direction.y);
        }
        public Vector2 GetVelocity() {
            if (native_body_ptr == IntPtr.Zero) return new Vector2(0.0f,0.0f);
            return PhysicsInterop.GetVelocity(native_body_ptr);
        }
        public float GetAngularVelocity() {
            if (native_body_ptr == IntPtr.Zero) return 0.0f;
            return PhysicsInterop.GetAngularVelocity(native_body_ptr);
        }
        public float GetGravityScale() {
            if (native_body_ptr == IntPtr.Zero) return gravity_scale;
            return PhysicsInterop.GetGravityScale(native_body_ptr);
        }
        public Vector2 GetUpDirection() { 
            if (native_body_ptr == IntPtr.Zero) return new Vector2(0.0f, 1.0f);
            return PhysicsInterop.GetUpDirection(native_body_ptr);
        }
        public Vector2 GetRightDirection() { 
            if (native_body_ptr == IntPtr.Zero) return new Vector2(1.0f, 0.0f);
            return PhysicsInterop.GetRightDirection(native_body_ptr);
        }
        public Vector2 GetPosition() {
            if (native_body_ptr == IntPtr.Zero) return new Vector2(0.0f, 0.0f);
            return PhysicsInterop.GetPosition(native_body_ptr);
        }
        public float GetRotation() {
            if (native_body_ptr == IntPtr.Zero) return 0.0f;
            return PhysicsInterop.GetRotation(native_body_ptr);
        }

        public void OnEarlyDataUpdate(int self_actor_id)
        {
            ref Position pos = ref Actor.GetComponent<Position>(self_actor_id);
            Vector2 rb_pos = PhysicsInterop.GetPosition(native_body_ptr);
            pos.x = rb_pos.x;
            pos.y = rb_pos.y;
        }
    }
}
