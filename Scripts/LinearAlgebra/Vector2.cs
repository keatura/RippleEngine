using System;
using System.Collections.Generic;
using System.Text;

namespace Engine.LinearAlgebra
{
    public class Vector2
    {
        public float x = 0;
        public float y = 0;

        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public Vector2()
        {
            this.x = 0;
            this.y = 0;
        }

        public Vector2(Vector2 other)
        {
            this.x = other.x;
            this.y = other.y;
        }

        public static Vector2 operator +(Vector2 a, Vector2 b) { return new Vector2(a.x + b.x, a.y + b.y); }
        public static Vector2 operator -(Vector2 a) { return new Vector2(-a.x, -a.y); }
        public static Vector2 operator -(Vector2 a, Vector2 b) { return new Vector2(a.x - b.x, a.y - b.y); }
        public static Vector2 operator *(Vector2 a, float scalar) { return new Vector2(a.x * scalar, a.y * scalar); }
        public static Vector2 operator *(float scalar, Vector2 a) { return new Vector2(a.x * scalar, a.y * scalar); }
        public static Vector2 operator *(Vector2 a, Vector2 b) { return new Vector2(a.x * b.x, a.y * b.y); } // hadamard operation

        public float Dot(Vector2 other) { return this.x * other.x + this.y * other.y; }
        public float LengthSquared() { return this.Dot(this); }
        public float Length() { return (float)Math.Sqrt(this.Dot(this)); }
        public float Cross(Vector2 other) { return (this.x * other.y) - (this.y * other.x); }
        public Vector2 Normalized()
        {
            float modifier = this.LengthSquared();
            if (modifier == 0.0f) return new Vector2();
            modifier = 1.0f / (float)Math.Sqrt(modifier);
            return this * modifier;
        }

        public Vector2 YX() { return new Vector2(this.y, this.x); }
        public Vector2 Lerp(Vector2 other, float delta) {
            return this + (other - this) * delta;
        }
        public static bool operator==(Vector2 a, Vector2 b) { return a.x == b.x && a.y == b.y; }
        public static bool operator!=(Vector2 a, Vector2 b) { return !(a == b); }

        public override bool Equals(object obj)
        {
            return base.Equals(obj);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override string ToString()
        {
            return $"X: {x} Y: {y}";
        }
    }
}
