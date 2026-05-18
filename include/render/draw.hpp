#pragma once
#include "assets/texture.hpp"
#include "math/vec2.hpp"
#include "math/vertex.hpp"

namespace astra::render {
    void drawTriangle(const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                      const math::Color &color);

    void drawRect(const math::Vec2 &pos, const math::Vec2 &size, const math::Color &color);

    void drawRect(const math::Vec2 &pos, const math::Vec2 &size, const assets::Texture &texture);

    void drawCircle(const math::Vec2 &pos, uint32_t r, const math::Color &color);

    void drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color);
}
