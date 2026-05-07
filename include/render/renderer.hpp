#pragma once
#include "framebuffer.hpp"
#include "math/point.hpp"
#include "math/vec2.hpp"

namespace astra::render {
class Renderer {
    Framebuffer framebuffer;
    math::Vec2 dpiScale;

  public:
    Renderer(const math::Point& windowSizeInPixels, const math::Vec2& dpiScale);

    void drawTriangle(const math::Vec2& a, const math::Vec2& b, const math::Vec2& c, const math::Color& color);

    void drawRect(const math::Vec2& pos, const math::Vec2& size, const math::Color& color);

    void drawCircle(const math::Vec2& pos, uint32_t r, const math::Color& color);

    void drawLine(const math::Vec2& a, const math::Vec2& b, const math::Color& color);

    void clear(math::Color color);

    void onWindowResize(const math::Point& newWindowSizeInPixels, const math::Vec2& newDpiScale);

    [[nodiscard]] const Buffer& getBuffer() const;
};
}
