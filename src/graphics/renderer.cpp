#include <numbers>

#include <astra/math/vector.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/graphics/texture.hpp>


namespace astra::graphics {
    Renderer::Renderer(const math::Point &windowSizeInPixels, const math::Vec2 &dpiScale) : dpiScale(dpiScale) {
    }

    void Renderer::drawTriangle(const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                                const math::Color &color) const {
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const math::Color &color) const {
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const Texture &texture) const {
    }

    void Renderer::drawCircle(const math::Vec2 &pos, const uint32_t r, const math::Color &color) const {
    }

    void Renderer::drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color) const {
    }

    void Renderer::onWindowResize(const math::Vec2 &newSize) {
    }
}
