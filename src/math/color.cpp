#include "math/color.hpp"

namespace astra::math{
    Color::Color(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b), a(255) {
    }

    Color::Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : r(r), g(g), b(b), a(a) {
    }

    uint32_t Color::wrap() const {
        return r << 24 | g << 16 | b << 8 | a;
    }

    Color Color::black() {
        return {0, 0, 0, 255};
    }

    Color Color::white() {
        return {255, 255, 255, 255};
    }

    Color Color::red() {
        return {255, 0, 0, 255};
    }

    Color Color::green() {
        return {0, 255, 0, 255};
    }

    Color Color::blue() {
        return {0, 0, 255, 255};
    }
}
