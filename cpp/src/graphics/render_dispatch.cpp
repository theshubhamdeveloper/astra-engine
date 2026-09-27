#include <algorithm>

#include <astra/graphics/render_dispatch.hpp>

namespace astra::graphics::dispatch {
    void shape(Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::RectGeometry &rectangleGeometry, const ecs::components::ShapeStyle &style) {
        renderer.drawRect({
            .position = transform.position,
            .size = {
                rectangleGeometry.size.x * transform.scale.x,
                rectangleGeometry.size.y * transform.scale.y
            },
            .rotation = transform.rotation,
            .style = {
                .cornerRadius = rectangleGeometry.cornerRadius,
                .strokeWidth = style.strokeWidth,
                .strokeColor = style.stroke,
                .texture = style.backgroundImage
            }
        });
    }

    void shape(Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::CircleGeometry &circleGeometry, const ecs::components::ShapeStyle &style) {
        renderer.drawRect({
            .position = transform.position,
            .size = {
                circleGeometry.radius * transform.scale.x,
                circleGeometry.radius * transform.scale.y
            },
            .rotation = transform.rotation,
            .style = {
                .cornerRadius = math::vec4{(circleGeometry.radius + style.strokeWidth * 2) / 2},
                .strokeWidth = style.strokeWidth,
                .strokeColor = style.stroke,
                .texture = style.backgroundImage
            }
        });
    }

    void shape(Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::LineGeometry &lineGeometry, const ecs::components::ShapeStyle &style) {
        // renderer.drawLine(lineGeometry.start, lineGeometry.start, style.backgroundColor);
    }
}
