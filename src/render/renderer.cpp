#include "render/renderer.hpp"
#include "render/draw.hpp"

namespace astra::render {
Renderer::Renderer(uint32_t width, uint32_t height) : framebuffer({width, height}) {}

void Renderer::drawTriangle(const math::Vec2& a, const math::Vec2& b, const math::Vec2& c, const math::Color& color) {
    render::drawTriangle(framebuffer, a, b, c, color);
}

void Renderer::drawRect(const math::Vec2& pos, const math::Vec2& size, const math::Color& color) {
    render::drawRect(framebuffer, pos, size, color);
}

void Renderer::drawCircle(const math::Vec2& pos, const uint32_t r, const math::Color& color) {
    render::drawCircle(framebuffer, pos, r, color);
}

void Renderer::drawLine(const math::Vec2& a, const math::Vec2& b, const math::Color& color) {
    render::drawLine(framebuffer, a, b, color);
}

void Renderer::clear(const math::Color color) {
    framebuffer.clear(color);
}

const Buffer& Renderer::getBuffer() const {
    return framebuffer.getBuffer();
}
}
