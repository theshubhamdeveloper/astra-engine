#pragma once

#include <cstdint>

#include <astra/math/vector.hpp>

namespace astra::ecs::components {
    struct Transform {
        math::Vec2 position;
        math::Vec2 scale;
        float rotation = 0.0f;
        int32_t zindex = 0;
    };
}
