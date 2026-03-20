#include <ranges>
#include "renderer/framebuffer.hpp"

namespace astra::renderer {
    Framebuffer::Framebuffer(const uint32_t width, const uint32_t height) : width(width), height(height) {
        framebuffer.resize(width * height, Color::black().wrap());
    }

    void Framebuffer::putPixel(const uint32_t x, const uint32_t y, const Color color) {
        if (x >= width || y >= height) return;
        framebuffer[y * width + x] = color.wrap();
    }

    void Framebuffer::clear(const Color color) {
        std::ranges::fill(framebuffer, color.wrap());
    }

    const Buffer &Framebuffer::getBuffer() const {
        return framebuffer;
    }
}
