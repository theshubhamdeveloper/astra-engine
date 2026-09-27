#pragma once

#include <cstdint>
#include <numbers>
#include <memory>

namespace astra::core {
    constexpr size_t INVALID_INDEX = SIZE_MAX;
    constexpr float RADIAN_CONVERSION_FACTOR = std::numbers::pi / 180.0f;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args &&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    };

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args &&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}
