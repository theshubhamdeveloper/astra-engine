#pragma once

#include <astra/ecs/components/shape.hpp>
#include <astra/ecs/components/transform.hpp>
#include <astra/graphics/renderer.hpp>

namespace astra::graphics::dispatch {
    void shape(Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::RectGeometry &rectangleGeometry, const ecs::components::ShapeStyle &style);

    void shape(Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::CircleGeometry &circleGeometry, const ecs::components::ShapeStyle &style);

    void shape(Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::LineGeometry &lineGeometry, const ecs::components::ShapeStyle &style);
}
