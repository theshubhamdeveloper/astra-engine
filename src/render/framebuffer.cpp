#include "render/framebuffer.hpp"
#include "math/point.hpp"

namespace astra::render {
Framebuffer::Framebuffer(const math::Point& size) : size(size) {
    framebuffer.resize(size.x * size.y, math::Color::black().wrap());
}

void Framebuffer::putPixel(const math::Point& position, const math::Color& color) {
    if (position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y)
        return;

    framebuffer[position.y * size.x + position.x] = color.wrap();
}

void Framebuffer::clear(const math::Color& color) {
    std::ranges::fill(framebuffer, color.wrap());
}

const Buffer& Framebuffer::getBuffer() const {
    return framebuffer;
}
}
