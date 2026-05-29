#pragma once

#include <vector>

#include <astra/core/assert.hpp>
#include <astra/graphics/texture.hpp>

namespace astra::graphics {
    struct TextureHandel {
        uint32_t id;
    };

    class TextureManager {
        std::vector<Texture> textures;

    public:
        TextureHandel loadTexture(const std::string &filePath) {
            textures.emplace_back(assets::Image::load(filePath));
            return {static_cast<uint32_t>(textures.size()) - 1};
        }

        [[nodiscard]] const Texture &getTexture(const TextureHandel &textureHandel) const {
            ASSERT(textures.size() > textureHandel.id);
            return textures.at(textureHandel.id);
        }
    };
}
