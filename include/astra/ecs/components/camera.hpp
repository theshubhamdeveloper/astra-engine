#pragma once

#include <astra/math/vector.hpp>

namespace astra::ecs::components {
    struct Camera {
        math::Vec2 position;
        float zoom = 1;
    };
}
