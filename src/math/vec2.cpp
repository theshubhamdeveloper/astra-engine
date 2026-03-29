#include "math/vec2.hpp"

namespace astra::math{
    Vec2::Vec2(const double x, const double y) : x(x), y(y) {
    }

    void Vec2::perpendicular() {
        const double tempX = x;
        x = -y;
        y = tempX;
    }

    double Vec2::dot(const Vec2 a, const Vec2 b) {
        return a.x * b.x + a.y * b.y;
    }

    Vec2 Vec2::zero() {
        return {0.0, 0.0};
    }

    Vec2 Vec2::one() {
        return {1.0, 1.0};
    }

    Vec2 Vec2::operator+(const Vec2 b) const {
        return {x + b.x, y + b.y};
    }

    Vec2 Vec2::operator-(const Vec2 b) const {
        return {x - b.x, y - b.y};
    }

    Vec2 Vec2::operator*(const double scalar) const {
        return {x * scalar, y * scalar};
    }

    Vec2 Vec2::operator/(const double scalar) const {
        return {x / scalar, y / scalar};
    }

    bool Vec2::operator==(const Vec2 b) const {
        return x == b.x && y == b.y;
    }
}
