#pragma once
#include "ecs/components/shape.hpp"
#include "ecs/components/transform.hpp"
#include "render/renderer.hpp"

namespace astra::render::dispatch {
    void shape(Renderer &renderer, const ecs::component::Transform &transform,
               const ecs::component::RectangleGeometry &rectangleGeometry, const ecs::component::ShapeStyle &style);

    void shape(Renderer &renderer, const ecs::component::Transform &transform,
               const ecs::component::CircleGeometry &circleGeometry, const ecs::component::ShapeStyle &style);

    void shape(Renderer &renderer, const ecs::component::Transform &transform,
               const ecs::component::TriangleGeometry &triangleGeometry, const ecs::component::ShapeStyle &style);

    void shape(Renderer &renderer, const ecs::component::Transform &transform,
               const ecs::component::LineGeometry &lineGeometry, const ecs::component::ShapeStyle &style);
}