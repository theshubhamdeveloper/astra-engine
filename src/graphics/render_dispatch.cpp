#include <algorithm>

#include <astra/graphics/render_dispatch.hpp>

namespace astra::graphics::dispatch {
    void shape(const Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::RectGeometry &rectangleGeometry, const ecs::components::ShapeStyle &style) {
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

    void shape(const Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::CircleGeometry &circleGeometry, const ecs::components::ShapeStyle &style) {
        renderer.drawCircle(transform.position,
                            static_cast<u_int32_t>(circleGeometry.radius * std::max(
                                                       transform.scale.x, transform.scale.y)),
                            style.backgroundColor);
    }

    void shape(const Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::TriangleGeometry &triangleGeometry, const ecs::components::ShapeStyle &style) {
        renderer.drawTriangle(triangleGeometry.a + transform.position,
                              triangleGeometry.b + transform.position,
                              triangleGeometry.c + transform.position, style.backgroundColor);
    }

    void shape(const Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::LineGeometry &lineGeometry, const ecs::components::ShapeStyle &style) {
        renderer.drawLine(lineGeometry.start, lineGeometry.start, style.backgroundColor);
    }
}
