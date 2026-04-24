#pragma once
#include "vec2.hpp"
#include <vector>

namespace astra::math {
bool isPointRightSideOfLine(const Vec2 &a, const Vec2 &b, const Vec2 &p);

bool isPointInsideTriangle(const Vec2 &a, const Vec2 &b, const Vec2 &c,
                           const Vec2 &p);

std::vector<Vec2> generateCircleVertices(const Vec2 &pos, uint32_t r,
                                         uint32_t segment);
}
