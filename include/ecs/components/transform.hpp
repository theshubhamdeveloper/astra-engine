#pragma once
#include "math/vec2.hpp"
#include <cstdint>

namespace astra::ecs::component {
    struct Transform {
        math::Vec2 position;
        math::Vec2 scale;
        float rotation;
        int32_t zindex;
    };
}