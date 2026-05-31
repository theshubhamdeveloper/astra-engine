#include <algorithm>

#include <astra/graphics/render_dispatch.hpp>

namespace astra::graphics::dispatch {
    void shape(Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::RectGeometry &rectangleGeometry, const ecs::components::ShapeStyle &style) {
        if (style.backgroundImage.id != 0) {
            renderer.drawRect(transform.position,
                              {
                                  rectangleGeometry.size.x * transform.scale.x,
                                  rectangleGeometry.size.y * transform.scale.y
                              },
                              transform.rotation, rectangleGeometry.cornerRadius,
                              style.backgroundImage);
            return;
        }

        renderer.drawRect(transform.position,
                          {
                              rectangleGeometry.size.x * transform.scale.x,
                              rectangleGeometry.size.y * transform.scale.y
                          },
                          transform.rotation, rectangleGeometry.cornerRadius,
                          style.backgroundColor);
    }

    void shape(Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::CircleGeometry &circleGeometry, const ecs::components::ShapeStyle &style) {
        if (style.backgroundImage.id != 0) {
            renderer.drawRect(transform.position,
                              {
                                  circleGeometry.radius * transform.scale.x,
                                  circleGeometry.radius * transform.scale.y
                              },
                              transform.rotation, math::Vec4{circleGeometry.radius / 2},
                              style.backgroundImage);
            return;
        }

        renderer.drawRect(transform.position,
                          {
                              circleGeometry.radius * transform.scale.x,
                              circleGeometry.radius * transform.scale.y
                          },
                          transform.rotation, math::Vec4{circleGeometry.radius / 2},
                          style.backgroundColor);
    }


    void shape(Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::TriangleGeometry &triangleGeometry, const ecs::components::ShapeStyle &style) {
        // renderer.drawTriangle(triangleGeometry.a + transform.position,
        //                       triangleGeometry.b + transform.position,
        //                       triangleGeometry.c + transform.position, style.backgroundColor);
    }

    void shape(Renderer &renderer, const ecs::components::Transform &transform,
               const ecs::components::LineGeometry &lineGeometry, const ecs::components::ShapeStyle &style) {
        // renderer.drawLine(lineGeometry.start, lineGeometry.start, style.backgroundColor);
    }
}
