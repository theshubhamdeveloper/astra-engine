#pragma once

#include <cstdint>

#include <astra/math/vec2.hpp>

namespace astra::ecs::components {
    struct Transform {
        math::Vec2 position;
        math::Vec2 scale;
        float rotation;
        int32_t zindex;
    };
}
