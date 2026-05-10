#pragma once
#include "color.hpp"
#include "vec2.hpp"

namespace astra::math {
    struct Vertex {
        Vec2 position;
        Vec2 uv;
        Color color;
    };
}