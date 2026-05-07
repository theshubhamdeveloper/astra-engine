#pragma once
#include "math/color.hpp"
#include "math/point.hpp"
#include <vector>

namespace astra::render {
using Buffer = std::vector<uint32_t>;

class Framebuffer {
    Buffer framebuffer;
    math::Point size;

  public:
    Framebuffer(const math::Point& size);

    void putPixel(const math::Point& position, const math::Color& color);

    void clear(const math::Color& color);

    void resizeBuffer(const math::Point& newSize);

    [[nodiscard]] const Buffer& getBuffer() const;
};
}
