#pragma once
#include "vec2.hpp"
#include <numbers>
#include <vector>

namespace astra::math {
    constexpr double TWO_PI = 2.0 * std::numbers::pi;

    constexpr float triangleEdge(const Vec2 &a, const Vec2 &b, const Vec2 &c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    constexpr bool isPointRightSideOfLine(const Vec2 &a, const Vec2 &b, const Vec2 &p) {
        return triangleEdge(a, b, p) < 0;
    }

    inline bool isPointInsideTriangle(const Vec2 &a, const Vec2 &b, const Vec2 &c, const Vec2 &p) {
        const bool sideAB = isPointRightSideOfLine(a, b, p);
        const bool sideBC = isPointRightSideOfLine(b, c, p);
        const bool sideCA = isPointRightSideOfLine(c, a, p);
        return sideAB == sideBC && sideAB == sideCA;
    }

    std::vector<Vec2> generateCircleVertices(const Vec2 &pos, uint32_t r, uint32_t segment);
}