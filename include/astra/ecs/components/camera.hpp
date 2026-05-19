#pragma once

#include <astra/math/vec2.hpp>

namespace astra::ecs::components {
    struct Viewport {
        math::Vec2 position;
        math::Vec2 size;
    };

    struct Camera {
        math::Vec2 position;
        float zoom = 1;
        Viewport viewport;
    };
}
