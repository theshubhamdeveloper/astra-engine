#pragma once
#include "math/vec2.hpp"

namespace astra::ecs::component {

struct Viewport {
    math::Vec2 position;
    math::Vec2 size;
};

struct Camera {
    math::Vec2 position;
    float zoom;
    Viewport viewport;
};
}
