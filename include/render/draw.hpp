#pragma once
#include <vector>
#include "framebuffer.hpp"
#include "math/vec2.hpp"

namespace astra::render {
    void drawTriangle(Framebuffer &fb, math::Vec2 a, math::Vec2 b, math::Vec2 c, math::Color color);

    void drawTriangleFan(Framebuffer &fb, const std::vector<math::Vec2> &vertices, math::Color color);

    void drawTriangleStrip(Framebuffer &fb, const std::vector<math::Vec2> &vertices, math::Color color);

    void drawRect(Framebuffer &fb, math::Vec2 pos, math::Vec2 size, math::Color color);

    void drawCircle(Framebuffer &fb, math::Vec2 pos, uint32_t r, math::Color color, const uint32_t segment);

    void drawLine(Framebuffer &fb, math::Vec2 a, math::Vec2 b, math::Color color);
}
