#pragma once
#include "fragment.hpp"
#include "framebuffer.hpp"
#include "assets/texture.hpp"
#include "math/vec2.hpp"
#include "math/vertex.hpp"

namespace astra::render {
    constexpr uint32_t superSamplingLevel = 2;
    constexpr float subPixelStep = 1.0f / (static_cast<float>(superSamplingLevel) + 1.0f);
    constexpr float totalSamples = static_cast<float>(superSamplingLevel * superSamplingLevel);

    void rasterizeTriangle(Framebuffer &fb,
                           const math::Vertex &a, const math::Vertex &b, const math::Vertex &c,
                           const FragmentShader &fragmentShader);

    void drawTriangle(Framebuffer &fb, const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                      const math::Color &color);

    void drawTriangleFan(Framebuffer &fb, const std::vector<math::Vertex> &vertices,
                         const FragmentShader &fragmentShader);

    void drawTriangleStrip(Framebuffer &fb, const std::vector<math::Vertex> &vertices,
                           const FragmentShader &fragmentShader);

    void drawRect(Framebuffer &fb, const math::Vec2 &pos, const math::Vec2 &size, const math::Color &color,
                  const FragmentShader &fragmentShader);

    void drawRect(Framebuffer &fb, const math::Vec2 &pos, const math::Vec2 &size, const math::Color &color);

    void drawRect(Framebuffer &fb, const math::Vec2 &pos, const math::Vec2 &size, const assets::Texture &texture);

    void drawCircle(Framebuffer &fb, const math::Vec2 &pos, uint32_t r, const math::Color &color,
                    uint32_t segment = 128);

    void drawLine(Framebuffer &fb, const math::Vec2 &a, const math::Vec2 &b, const math::Color &color);
}
