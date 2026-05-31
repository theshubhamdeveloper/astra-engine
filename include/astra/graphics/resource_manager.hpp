#pragma once

#include <vector>

#include <astra/core/assert.hpp>
#include <astra/graphics/shader.hpp>
#include <astra/graphics/texture.hpp>

namespace astra::graphics {
    struct ShaderHandel {
        uint32_t id = 0;
    };

    struct TextureHandel {
        uint32_t id = 0;
    };

    class ResourceManager {
        std::vector<Shader> shaders;
        std::vector<Texture> textures;
        std::string resourceRootPath;

    public:
        ResourceManager() : shaders(1), textures(1) {
        }

        void setResourceRootPath(const std::string &path) {
            resourceRootPath = path;
        }

        ShaderHandel loadShader(const std::string &vertFilePath, const std::string &fragFilePath) {
            shaders.emplace_back(resourceRootPath + vertFilePath, resourceRootPath + fragFilePath);
            return {static_cast<uint32_t>(shaders.size()) - 1};
        }

        [[nodiscard]] const Shader &getShader(const ShaderHandel &shaderHandel) const {
            ASSERT(shaders.size() > shaderHandel.id || shaderHandel.id > 0);
            return shaders.at(shaderHandel.id);
        }

        TextureHandel loadTexture(const std::string &filePath) {
            textures.emplace_back(assets::Image::load(resourceRootPath + filePath));
            return {static_cast<uint32_t>(textures.size()) - 1};
        }

        TextureHandel loadTexture(const assets::Image &image) {
            textures.emplace_back(image);
            return {static_cast<uint32_t>(textures.size()) - 1};
        }

        [[nodiscard]] const Texture &getTexture(const TextureHandel &textureHandel) const {
            ASSERT(shaders.size() > textureHandel.id || textureHandel.id > 0);
            return textures.at(textureHandel.id);
        }
    };
}
