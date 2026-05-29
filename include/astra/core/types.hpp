#pragma once

#include <cstdint>
#include <numbers>

namespace astra::core {
    constexpr size_t INVALID_INDEX = SIZE_MAX;
    constexpr float RADIAN_CONVERSION_FACTOR = std::numbers::pi / 180.0f;
}
