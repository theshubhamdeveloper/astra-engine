#pragma once

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
    };

    using ShaderHandle = Handle<ShaderTag>;
    using TextureHandle = Handle<TextureTag>;
    using FontHandle = Handle<FontTag>;
}
