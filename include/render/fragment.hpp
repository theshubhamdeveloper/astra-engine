#pragma once
#include "math/color.hpp"
#include "math/vec2.hpp"
#include <functional>

namespace astra::render {
    struct Fragment {
        math::Vec2 uv;
        math::Color color;
    };

    using FragmentShader = std::function<math::Color(const Fragment &fragment)>;
}
