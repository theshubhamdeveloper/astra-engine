#pragma once

#include "vec2.hpp"

namespace astra::math {
    constexpr float triangleEdge(const Vec2 &a, const Vec2 &b, const Vec2 &c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    inline bool isPointInsideTriangle(const Vec2 &a, const Vec2 &b, const Vec2 &c, const Vec2 &p) {
        const bool sideAB = triangleEdge(a, b, p) <= 0;
        const bool sideBC = triangleEdge(b, c, p) <= 0;
        const bool sideCA = triangleEdge(c, a, p) <= 0;
        return sideAB == sideBC && sideAB == sideCA;
    }
}
