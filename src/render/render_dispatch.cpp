#include "render/render_dispatch.hpp"
#include <algorithm>

namespace astra::render::dispatch {
    void shape(Renderer &renderer, const ecs::component::Transform &transform,
               const ecs::component::RectGeometry &rectangleGeometry, const ecs::component::ShapeStyle &style) {
        if (style.backgroundImage.has_value()) {
            renderer.drawRect(transform.position,
                              {
                                  rectangleGeometry.size.x * transform.scale.x,
                                  rectangleGeometry.size.y * transform.scale.y
                              },
                              style.backgroundImage.value());
            return;
        }

        renderer.drawRect(transform.position,
                          {rectangleGeometry.size.x * transform.scale.x, rectangleGeometry.size.y * transform.scale.y},
                          style.backgroundColor);
    }

    void shape(Renderer &renderer, const ecs::component::Transform &transform,
               const ecs::component::CircleGeometry &circleGeometry, const ecs::component::ShapeStyle &style) {
        renderer.drawCircle(transform.position,
                            static_cast<u_int32_t>(circleGeometry.radius * std::max(
                                                       transform.scale.x, transform.scale.y)),
                            style.backgroundColor);
    }

    void shape(Renderer &renderer, const ecs::component::Transform &transform,
               const ecs::component::TriangleGeometry &triangleGeometry, const ecs::component::ShapeStyle &style) {
        renderer.drawTriangle(triangleGeometry.a + transform.position,
                              triangleGeometry.b + transform.position,
                              triangleGeometry.c + transform.position, style.backgroundColor);
    }

    void shape(Renderer &renderer, const ecs::component::Transform &transform,
               const ecs::component::LineGeometry &lineGeometry, const ecs::component::ShapeStyle &style) {
        renderer.drawLine(lineGeometry.start, lineGeometry.start, style.backgroundColor);
    }
}
