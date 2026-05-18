#include "render/renderer.hpp"
#include "assets/texture.hpp"
#include "math/point.hpp"
#include "math/vec2.hpp"
#include "render/draw.hpp"

namespace astra::render {
    Renderer::Renderer(const math::Point &windowSizeInPixels, const math::Vec2 &dpiScale) : dpiScale(dpiScale) {
    }

    void Renderer::drawTriangle(const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                                const math::Color &color) const {
        render::drawTriangle(a * dpiScale, b * dpiScale, c * dpiScale, color);
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const math::Color &color) const {
        render::drawRect(pos * dpiScale, size * dpiScale, color);
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const assets::Texture &texture) const {
        render::drawRect(pos * dpiScale, size * dpiScale, texture);
    }

    void Renderer::drawCircle(const math::Vec2 &pos, const uint32_t r, const math::Color &color) const {
        render::drawCircle(pos * dpiScale, r * dpiScale.x, color);
    }

    void Renderer::drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color) const {
        render::drawLine(a * dpiScale, b * dpiScale, color);
    }

    void Renderer::onWindowResize(const math::Point &newWindowSizeInPixels, const math::Vec2 &newDpiScale) {
        dpiScale = newDpiScale;
    }
}
