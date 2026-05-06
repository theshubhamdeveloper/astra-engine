#include "math/geometry.hpp"

namespace astra::math {
std::vector<Vec2> generateCircleVertices(const Vec2& pos, const uint32_t r, const uint32_t segment) {
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
