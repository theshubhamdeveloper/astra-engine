#pragma once

#include <cstdint>

#include <astra/math/vector.hpp>

namespace astra::ecs::components {
    struct Transform {
        math::vec2 position;
        math::vec2 scale = math::vec2::one();
        float rotation = 0.0f;
        int32_t zindex = 0;
    };
}
