#pragma once
#include "math/color.hpp"
#include "math/vec2.hpp"
#include <variant>

namespace astra::ecs::component {
    struct RectangleGeometry {
        math::Vec2 size;
        float cornerRadius;
    };

    struct CircleGeometry {
        float radius;
    };

    struct LineGeometry {
        math::Vec2 start;
        math::Vec2 end;
    };

    struct TriangleGeometry {
        math::Vec2 a;
        math::Vec2 b;
        math::Vec2 c;
    };

    using ShapeGeometry = std::variant<RectangleGeometry, CircleGeometry, LineGeometry, TriangleGeometry>;

    struct ShapeStyle {
        math::Color fill;
        math::Color stroke;
        float strokeWidth;
        float opacity;
        bool display;
    };

    struct Shape {
        ShapeGeometry geometry;
        ShapeStyle style;
    };
}
