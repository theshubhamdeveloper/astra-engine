#pragma once

#include <astra/math/color.hpp>
#include <astra/math/vector.hpp>

namespace astra::math {
    struct Vertex {
        Vec2 position;
        Vec2 uv;
        Color color;
        float texId = 0;
    };
}
