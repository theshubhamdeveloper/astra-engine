#pragma once

#include <astra/math/vector.hpp>

namespace astra::math {
    constexpr float triangleEdge(const vec2 &a, const vec2 &b, const vec2 &c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    inline bool isPointInsideTriangle(const vec2 &a, const vec2 &b, const vec2 &c, const vec2 &p) {
        const bool sideAB = triangleEdge(a, b, p) <= 0;
        const bool sideBC = triangleEdge(b, c, p) <= 0;
        const bool sideCA = triangleEdge(c, a, p) <= 0;
        return sideAB == sideBC && sideAB == sideCA;
    }
}
