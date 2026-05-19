#pragma once

#include <cstdint>

#include <astra/math/vec2.hpp>

namespace astra::math {
    struct Point {
        int32_t x = 0;
        int32_t y = 0;

        constexpr Point() = default;

        constexpr Point(int32_t x, int32_t y) : x(x), y(y) {
        }

        inline math::Vec2 toVec2() const {
            return {static_cast<float>(x), static_cast<float>(y)};
        }

        inline constexpr Point operator+(const Point &other) const {
            return {x + other.x, y + other.y};
        }

        inline constexpr Point operator-(const Point &other) const {
            return {x - other.x, y - other.y};
        }

        inline constexpr Point operator*(int32_t scalar) const {
            return {x * scalar, y * scalar};
        }

        inline constexpr Point operator/(int32_t scalar) const {
            return {x / scalar, y / scalar};
        }

        inline Point &operator+=(const Point &other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        inline Point &operator-=(const Point &other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        inline Point &operator*=(int32_t scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        inline Point &operator/=(int32_t scalar) {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        inline constexpr bool operator==(const Point &other) const {
            return x == other.x && y == other.y;
        }

        inline constexpr bool operator!=(const Point &other) const {
            return !(*this == other);
        }
    };
}
