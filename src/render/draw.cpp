#include "render/draw.hpp"
#include "assets/texture.hpp"
#include "math/geometry.hpp"
#include "math/vertex.hpp"
using astra::math::Vec2;

namespace astra::render {
    void drawTriangle(const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                      const math::Color &color) {
    }

    void drawRect(const Vec2 &pos, const Vec2 &size, const math::Color &color) {
    }

    void drawRect(const math::Vec2 &pos, const math::Vec2 &size, const assets::Texture &texture) {
    }

    void drawCircle(const Vec2 &pos, const uint32_t r, const math::Color &color) {
    }

    void drawLine(const Vec2 &a, const Vec2 &b, const math::Color &color) {
    }
}
