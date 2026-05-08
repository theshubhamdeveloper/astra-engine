#include "render/framebuffer.hpp"
#include "math/color.hpp"
#include "math/point.hpp"

namespace astra::render {
    Framebuffer::Framebuffer(const math::Point &size) : size(size) {
        framebuffer.resize(size.x * size.y, math::Color::black().packRGBA());
    }

    void Framebuffer::putPixel(const math::Point &position, const math::Color &color) {
        if (position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y)
            return;

        uint32_t &pixel = framebuffer[position.y * size.x + position.x];

        pixel = math::Color::alphaBlend(math::Color::unpackUint32(pixel), color).packRGBA();
    }

    void Framebuffer::clear(const math::Color &color) {
        std::ranges::fill(framebuffer, color.packRGBA());
    }

    void Framebuffer::resizeBuffer(const math::Point &newSize) {
        size = newSize;
        framebuffer.resize(size.x * size.y, math::Color::black().packRGBA());
    }

    const Buffer &Framebuffer::getBuffer() const {
        return framebuffer;
    }
}