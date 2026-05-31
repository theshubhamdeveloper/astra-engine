#pragma once

#include <astra/math/vector.hpp>

namespace astra::ecs::components {
    struct Camera {
        math::Vec2 position;
        float zoom = 1.0f;
        float maxZoom = 100.0f;
        float minZoom = 0.01f;
        float zoomSpeed = 1.0f;
        float moveSpeed = 1.0f;
    };
}
