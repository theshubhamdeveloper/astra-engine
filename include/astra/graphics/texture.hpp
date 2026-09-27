#pragma once

#include <astra/assets/image.hpp>

namespace astra::graphics {
    enum class TextureColorFormat {
        RGBA = GL_RGBA,
        Red = GL_RED
    };

    struct Texture {
        struct Desc {
            assets::Image image;
            TextureColorFormat colorFormat = TextureColorFormat::RGBA;
        };

        Texture() = default;

        explicit Texture(const Desc &desc);

        void setPixels(const assets::Image &image);

        void use(uint32_t unit) const;

    private :
        uint32_t id = 0;
    };
}
