#pragma once

#include <cmath>

namespace astra::math {
    struct Vec2 {
        float x = 0.0f;
        float y = 0.0f;

        constexpr Vec2() = default;

        constexpr Vec2(float x, float y) : x(x), y(y) {
        }

        // Static
        static constexpr Vec2 zero() {
            return {0.0f, 0.0f};
        }

        static constexpr Vec2 one() {
            return {1.0f, 1.0f};
        }

        static constexpr Vec2 up() {
            return {0.0f, -1.0f};
        }

        static constexpr Vec2 down() {
            return {0.0f, 1.0f};
        }

        static constexpr Vec2 left() {
            return {-1.0f, 0.0f};
        }

        static constexpr Vec2 right() {
            return {1.0f, 0.0f};
        }

        // Operators
        inline constexpr Vec2 operator+(const Vec2 &other) const {
            return {x + other.x, y + other.y};
        }

        inline constexpr Vec2 operator-(const Vec2 &other) const {
            return {x - other.x, y - other.y};
        }

        inline constexpr Vec2 operator*(const Vec2 &other) const {
            return {x * other.x, y * other.y};
        }

        inline constexpr Vec2 operator/(const Vec2 &other) const {
            return {x / other.x, y / other.y};
        }

        inline constexpr Vec2 operator*(float scalar) const {
            return {x * scalar, y * scalar};
        }

        inline constexpr Vec2 operator/(float scalar) const {
            return {x / scalar, y / scalar};
        }

        inline Vec2 &operator+=(const Vec2 &other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        inline Vec2 &operator-=(const Vec2 &other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        inline Vec2 &operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        inline Vec2 &operator/=(float scalar) {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        inline constexpr Vec2 operator-() const {
            return {-x, -y};
        }

        inline constexpr bool operator==(const Vec2 &other) const {
            return x == other.x && y == other.y;
        }

        inline constexpr bool operator!=(const Vec2 &other) const {
            return !(*this == other);
        }

        // Calculation
        inline float length() const {
            return std::sqrt(x * x + y * y);
        }

        inline constexpr float lengthSquared() const {
            return x * x + y * y;
        }

        inline Vec2 normalized() const {
            float len = length();

            if (len == 0.0f)
                return Vec2::zero();

            return {x / len, y / len};
        }

        inline void normalize() {
            float len = length();

            if (len == 0.0f)
                return;

            x /= len;
            y /= len;
        }

        inline constexpr float dot(const Vec2 &other) const {
            return x * other.x + y * other.y;
        }

        inline float distance(const Vec2 &other) const {
            return (*this - other).length();
        }

        inline constexpr float distanceSquared(const Vec2 &other) const {
            return (*this - other).lengthSquared();
        }
    };

    inline constexpr Vec2 operator*(float scalar, const Vec2 &vec) {
        return {vec.x * scalar, vec.y * scalar};
    }
}