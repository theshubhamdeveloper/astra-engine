#include "math/geometry.hpp"
#include <numbers>

namespace astra::math {
    bool isPointRightSideOfLine(const Vec2 a, const Vec2 b, const Vec2 p) {
        Vec2 ab = b - a;
        const Vec2 ap = p - a;

        ab.perpendicular();
        return Vec2::dot(ab, ap) < 0;
    }

    bool isPointInsideTriangle(const Vec2 a, const Vec2 b, const Vec2 c, const Vec2 p) {
        const bool sideAB = isPointRightSideOfLine(a, b, p);
        const bool sideBC = isPointRightSideOfLine(b, c, p);
        const bool sideCA = isPointRightSideOfLine(c, a, p);
        return sideAB == sideBC && sideBC == sideCA;
    }

    std::vector<Vec2> generateCircleVertices(const Vec2 pos, const uint32_t r, uint32_t segment) {
        std::vector<Vec2> points;
        Vec2 dir = Vec2::zero();
        double angle = 0;

        const double step = 2 * std::numbers::pi / segment;

        for (uint32_t i = 0; i < segment; i++) {
            angle = i * step;
            dir.x = r * std::cos(angle);
            dir.y = r * std::sin(angle);
            points.emplace_back(pos.x + dir.x, pos.y + dir.y);
        }

        return points;
    }
}
