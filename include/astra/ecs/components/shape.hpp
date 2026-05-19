#pragma once

#include <variant>

#include <astra/graphics/texture.hpp>
#include <astra/math/color.hpp>
#include <astra/math/vec2.hpp>

namespace astra::ecs::components {
    struct RectGeometry {
        math::Vec2 size;
        float cornerRadius = 0;
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

    using ShapeGeometry = std::variant<RectGeometry, CircleGeometry, LineGeometry, TriangleGeometry>;

    struct ShapeStyle {
        bool display = true;
        math::Color backgroundColor = math::Color::transparent();
        std::optional<graphics::Texture> backgroundImage = std::nullopt;
        math::Color stroke = math::Color::black();
        float strokeWidth = 0;
        float opacity = 1;
    };

    struct Shape {
        ShapeGeometry geometry;
        ShapeStyle style;
    };
}
