#pragma once

#include <astra/math/color.hpp>
#include <astra/math/vec2.hpp>

namespace astra::math {
    struct Vertex {
        Vec2 position;
        Vec2 uv;
        Color color;
    };
}
