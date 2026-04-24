#pragma once
#include "math/color.hpp"
#include <vector>

namespace astra::render {
using Buffer = std::vector<uint32_t>;

class Framebuffer {
    Buffer framebuffer;
    uint32_t width, height;

  public:
    Framebuffer(uint32_t width, uint32_t height);

    void putPixel(uint32_t x, uint32_t y, math::Color color);

    void clear(math::Color color);

    [[nodiscard]] const Buffer &getBuffer() const;
};
}
