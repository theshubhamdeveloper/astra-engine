#include "math/geometry.hpp"
#include <numbers>

namespace astra::math {
constexpr double TWO_PI = 2.0 * std::numbers::pi;

bool isPointRightSideOfLine(const Vec2 &a, const Vec2 &b, const Vec2 &p) {
    return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x) < 0;
}

bool isPointInsideTriangle(const Vec2 &a, const Vec2 &b, const Vec2 &c,
                           const Vec2 &p) {
    const bool sideAB = isPointRightSideOfLine(a, b, p);
    const bool sideBC = isPointRightSideOfLine(b, c, p);
    const bool sideCA = isPointRightSideOfLine(c, a, p);
    return sideAB == sideBC && sideAB == sideCA;
}

std::vector<Vec2> generateCircleVertices(const Vec2 &pos, const uint32_t r,
                                         const uint32_t segment) {
    std::vector<Vec2> points;
    points.reserve(segment);

    Vec2 dir = Vec2::zero();
    double angle = 0;

    const double step = TWO_PI / segment;

    for (uint32_t i = 0; i < segment; i++) {
        dir.x = r * std::cos(angle);
        dir.y = r * std::sin(angle);
        points.emplace_back(pos.x + dir.x, pos.y + dir.y);
        angle += step;
    }

    return points;
}
}
