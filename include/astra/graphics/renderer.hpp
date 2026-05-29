#pragma once

#include <astra/math/vector.hpp>
#include <astra/math/vertex.hpp>

namespace astra::graphics {
    class Renderer {
        math::Vec2 dpiScale;

    public:
        Renderer(const math::Point &windowSizeInPixels, const math::Vec2 &dpiScale);

        void drawTriangle(const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                          const math::Color &color) const;

        void drawRect(const math::Vec2 &pos, const math::Vec2 &size, const math::Color &color) const;

        void drawRect(const math::Vec2 &pos, const math::Vec2 &size, const Texture &texture) const;

        void drawCircle(const math::Vec2 &pos, uint32_t r, const math::Color &color) const;

        void drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color) const;

        void onWindowResize(const math::Vec2 &newSize);
    };
}
