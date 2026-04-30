#include "render/render_dispatch.hpp"

namespace astra::render::dispatch {
void shape(Renderer& renderer, const ecs::component::Transform& transform,
           const ecs::component::RectangleGeometry& rectangleGeometry, const ecs::component::ShapeStyle& style) {
    renderer.drawRect(transform.position, rectangleGeometry.size, style.fill);
}

void shape(Renderer& renderer, const ecs::component::Transform& transform,
           const ecs::component::CircleGeometry& circleGeometry, const ecs::component::ShapeStyle& style) {
    renderer.drawCircle(transform.position, circleGeometry.radius, style.fill);
}

void shape(Renderer& renderer, const ecs::component::Transform& transform,
           const ecs::component::LineGeometry& lineGeometry, const ecs::component::ShapeStyle& style) {
    renderer.drawLine(lineGeometry.start, lineGeometry.start, style.fill);
}
}
