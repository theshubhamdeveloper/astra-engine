#pragma once
#include <vector>

#include "vec2.hpp"

namespace astra::math {
    bool isPointRightSideOfLine(Vec2 a, Vec2 b, Vec2 p);

    bool isPointInsideTriangle(Vec2 a, Vec2 b, Vec2 c, Vec2 p);

    std::vector<Vec2> generateCircleVertices(Vec2 pos, uint32_t r, uint32_t segment);
}
