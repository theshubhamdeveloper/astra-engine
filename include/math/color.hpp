#pragma once
#include <cstdint>

namespace astra::math {

struct Color {
    uint8_t r, g, b, a;

    constexpr Color() : r(0), g(0), b(0), a(255) {}

    constexpr Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}

    static Color alphaBlend(const Color& dst, const Color& src);

    [[nodiscard]]
    constexpr uint32_t packRGBA() const {
        return (uint32_t(r) << 24) | (uint32_t(g) << 16) | (uint32_t(b) << 8) | uint32_t(a);
    }

    static constexpr Color unpackUint32(const uint32_t packedColor) {
        uint8_t r = (packedColor >> 24) & 0xFF;
        uint8_t g = (packedColor >> 16) & 0xFF;
        uint8_t b = (packedColor >> 8) & 0xFF;
        uint8_t a = packedColor & 0xFF;
        return {r, g, b, a};
    }

    static constexpr Color black() {
        return {0, 0, 0};
    }

    static constexpr Color white() {
        return {255, 255, 255};
    }

    static constexpr Color red() {
        return {255, 0, 0};
    }

    static constexpr Color green() {
        return {0, 255, 0};
    }

    static constexpr Color blue() {
        return {0, 0, 255};
    }

    static constexpr Color transparent() {
        return {0, 0, 0, 0};
    }
};

}
