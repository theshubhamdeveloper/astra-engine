#pragma once
#include <cstdint>

namespace astra {
    struct Color {
        uint8_t r, g, b, a;

        Color(uint8_t r, uint8_t g, uint8_t b);

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        [[nodiscard]] uint32_t wrap() const;

        static Color black();

        static Color white();

        static Color red();

        static Color green();

        static Color blue();
    };
}
