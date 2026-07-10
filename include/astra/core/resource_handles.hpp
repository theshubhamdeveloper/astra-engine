#pragma once

#include <cstdint>

namespace astra::core {
    struct TextureTag {
    };

    struct FontTag {
    };

    struct ShaderTag {
    };

    template<typename T>
    struct Handle {
        uint32_t id = 0;
        uint32_t generation = 0;

        constexpr explicit operator bool() const {
            return id != 0;
        }

        constexpr bool operator!() const noexcept {
            return id == 0;
        }

        constexpr bool operator==(const Handle &other) const noexcept {
            return id == other.id && generation == other.generation;
        }

        constexpr bool operator!=(const Handle &other) const noexcept {
            return !(*this == other);
        }
    };

    using ShaderHandle = Handle<ShaderTag>;
    using TextureHandle = Handle<TextureTag>;
    using FontHandle = Handle<FontTag>;
}
