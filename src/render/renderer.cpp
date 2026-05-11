#include "render/renderer.hpp"
#include "assets/texture.hpp"
#include "math/point.hpp"
#include "math/vec2.hpp"
#include "render/draw.hpp"

namespace astra::render {
    Renderer::Renderer(const math::Point &windowSizeInPixels, const math::Vec2 &dpiScale) : framebuffer({
        windowSizeInPixels
    }), dpiScale(dpiScale) {
    }

    void Renderer::drawTriangle(const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                                const math::Color &color) {
        render::drawTriangle(framebuffer, a * dpiScale, b * dpiScale, c * dpiScale, color);
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const math::Color &color) {
        render::drawRect(framebuffer, pos * dpiScale, size * dpiScale, color);
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const assets::Texture &texture) {
        render::drawRect(framebuffer, pos * dpiScale, size * dpiScale, texture);
    }

    void Renderer::drawCircle(const math::Vec2 &pos, const uint32_t r, const math::Color &color) {
        render::drawCircle(framebuffer, pos * dpiScale, r * dpiScale.x, color);
    }

    void Renderer::drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color) {
        render::drawLine(framebuffer, a * dpiScale, b * dpiScale, color);
    }

    void Renderer::clear(const math::Color color) {
        framebuffer.clear(color);
    }

    void Renderer::onWindowResize(const math::Point &newWindowSizeInPixels, const math::Vec2 &newDpiScale) {
        framebuffer.resizeBuffer(newWindowSizeInPixels);
        dpiScale = newDpiScale;
    }

    const Buffer &Renderer::getBuffer() const {
        return framebuffer.getBuffer();
    }
}