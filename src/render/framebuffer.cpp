#include "render/framebuffer.hpp"

namespace astra::render {
Framebuffer::Framebuffer(const uint32_t width, const uint32_t height)
    : width(width), height(height) {
    framebuffer.resize(width * height, math::Color::black().wrap());
}

void Framebuffer::putPixel(const uint32_t x, const uint32_t y,
                           const math::Color color) {
    if (x >= width || y >= height)
        return;
    framebuffer[y * width + x] = color.wrap();
}

void Framebuffer::clear(const math::Color color) {
    std::ranges::fill(framebuffer, color.wrap());
}

const Buffer &Framebuffer::getBuffer() const { return framebuffer; }
}
