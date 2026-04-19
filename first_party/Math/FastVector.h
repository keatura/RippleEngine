#pragma once

#include <cmath>
#include <cstdint>
#include <type_traits>
#include "glm/glm.hpp"

template <typename T>
struct alignas(sizeof(T) * 2) vec2 {
    union {
        struct { T x, y; };
        struct { T r, g; }; // useful for colors
        struct { T u, v; }; // useful for texture coords
        T data[2];
    };

    vec2() = default;
    constexpr vec2(T _x, T _y) : x(_x), y(_y) {}

    template <typename U>
    explicit constexpr vec2(const vec2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

    [[nodiscard]] inline constexpr vec2 operator+(vec2 other) const noexcept { return { x + other.x, y + other.y }; }
    [[nodiscard]] inline constexpr vec2 operator-() const noexcept { return { -x, -y }; }
    [[nodiscard]] inline constexpr vec2 operator-(vec2 other) const noexcept { return { x - other.x, y - other.y }; }
    [[nodiscard]] inline constexpr vec2 operator*(T scalar) const noexcept { return { x * scalar, y * scalar }; }
    [[nodiscard]] inline constexpr vec2 operator*(vec2 other) const noexcept { return { x * other.x, y * other.y }; } // hadamard

    friend inline constexpr vec2 operator*(T scalar, vec2 vec) { return vec * scalar; }

    inline constexpr vec2& operator+=(vec2 other) noexcept { x += other.x; y += other.y; return *this; }
    inline constexpr vec2& operator-=(vec2 other) noexcept { x -= other.x; y -= other.y; return *this; }
    inline constexpr vec2& operator*=(T scalar) noexcept { x *= scalar; y *= scalar; return *this; }

    [[nodiscard]] inline constexpr T dot(vec2 other) const noexcept { return (x * other.x) + (y * other.y); }
    [[nodiscard]] inline constexpr T length_sq() const noexcept { return (x * x) + (y * y); }

    [[nodiscard]] inline T length() const noexcept { return std::sqrt(length_sq()); }

    [[nodiscard]] inline constexpr T cross(vec2 other) const noexcept { return (x * other.y) - (y * other.x); }

    [[nodiscard]] inline vec2 normalized() const noexcept {
        static_assert(std::is_floating_point<T>::value, "Normalize requires floating point type");
        T len_sq = length_sq();
        if (len_sq < 1e-8) return { 0, 0 };
        T inv_len = static_cast<T>(1) / std::sqrt(len_sq);
        return { x * inv_len, y * inv_len };
    }

    [[nodiscard]] inline constexpr vec2 yx() const noexcept { return { y, x }; }
    [[nodiscard]] inline constexpr vec2 lerp(vec2 other, T t) const noexcept { return (*this) + (other - (*this)) * t; }

    [[nodiscard]] inline T& operator[](size_t i) noexcept { return data[i]; }
    [[nodiscard]] inline const T& operator[](size_t i) const noexcept { return data[i]; }

    [[nodiscard]] inline bool operator==(vec2 other) const { return x == other.x && y == other.y; }
    [[nodiscard]] inline bool operator!=(vec2 other) const { return !(*this == other); }

    /*
    [[nodiscard]] inline float dist_to_line(vec2 a, vec2 b) const noexcept {
        vec2 line = b - a;
        T area = (this->operator-(a)).cross(line);
        return glm::abs(static_cast<float>(area)) / std::sqrt(line.length_sq());
    }
     */
};

using ivec2 = vec2<int32_t>;
using fvec2 = vec2<float>;
using uivec2 = vec2<uint32_t>;

// helper for free-standing normalize
template <typename T>
[[nodiscard]] inline vec2<T> normalize(vec2<T> v) noexcept { return v.normalized(); }

template <typename T>
struct mat2 {
	vec2<T> col0;
	vec2<T> col1;

	static inline mat2 rotation(float radians) {
		T c = glm::cos(radians);
		T s = glm::sin(radians);
		return {
			{c, s},
			{-s ,c}
		};
	}

	// linear transformation via this matrix
	[[nodiscard]] inline constexpr vec2<T> operator*(vec2<T> v) const noexcept {
		return (col0 * v.x) + (col1 * v.y);
	}
	[[nodiscard]] inline constexpr mat2 operator*(T scalar) const noexcept {
		return {
			col0 * scalar,
			col1 * scalar
		};
	}
	[[nodiscard]] friend inline constexpr mat2 operator*(T scalar, mat2 m) noexcept {
		return m * scalar;
	}
	[[nodiscard]] inline constexpr T det() const noexcept { return col0.cross(col1); }
	[[nodiscard]] inline constexpr mat2 operator*(mat2 other) const noexcept {
		return {
			(*this) * other.col0,
			(*this) * other.col1
		};
	}
	[[nodiscard]] inline constexpr mat2 inverse() const noexcept {
		T d = det();
		if (d == 0) return { {0,0}, {0,0} }; // idk, undefined
		T inv_det = static_cast<T>(1) / d;
		return {
			{  col1.y * inv_det, -col0.y * inv_det },
			{ -col1.x * inv_det,  col0.x * inv_det }
		};
	}
};