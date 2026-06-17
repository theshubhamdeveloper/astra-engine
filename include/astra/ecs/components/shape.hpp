#pragma once

#include <variant>

#include <astra/core/resource_manager.hpp>
#include <astra/math/color.hpp>
#include <astra/math/vector.hpp>

namespace astra::ecs::components {
    struct RectGeometry {
        math::Vec2 size;
        math::Vec4 cornerRadius;
    };

    struct CircleGeometry {
        float radius;
    };

    struct LineGeometry {
        math::Vec2 start;
        math::Vec2 end;
    };

    using ShapeGeometry = std::variant<RectGeometry, CircleGeometry, LineGeometry>;

    struct ShapeStyle {
        bool display = true;
        math::Color backgroundColor = math::Color::transparent();
        core::TextureHandle backgroundImage;
        math::Color stroke = math::Color::black();
        float strokeWidth = 0;
        float opacity = 1;
    };

    struct Shape {
        ShapeGeometry geometry;
        ShapeStyle style;
    };
}
