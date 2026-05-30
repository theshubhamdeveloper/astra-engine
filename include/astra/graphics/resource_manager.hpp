#pragma once

#include <vector>

#include <astra/core/assert.hpp>
#include <astra/graphics/shader.hpp>
#include <astra/graphics/texture.hpp>

namespace astra::graphics {
    struct ShaderHandel {
        uint32_t id;
    };

    struct TextureHandel {
        uint32_t id;
    };

    class ResourceManager {
        std::vector<Shader> shaders;
        std::vector<Texture> textures;
        std::string resourceRootPath;

    public:
        void setResourceRootPath(const std::string &path) {
            resourceRootPath = path;
        }

        ShaderHandel loadShader(const std::string &vertFilePath, const std::string &fragFilePath) {
            shaders.emplace_back(resourceRootPath + vertFilePath, resourceRootPath + fragFilePath);
            return {static_cast<uint32_t>(shaders.size()) - 1};
        }

        [[nodiscard]] const Shader &getShader(const ShaderHandel &shaderHandel) const {
            ASSERT(shaders.size() > shaderHandel.id);
            return shaders.at(shaderHandel.id);
        }

        TextureHandel loadTexture(const std::string &filePath) {
            textures.emplace_back(assets::Image::load(resourceRootPath + filePath));
            return {static_cast<uint32_t>(textures.size()) - 1};
        }

        [[nodiscard]] const Texture &getTexture(const TextureHandel &textureHandel) const {
            ASSERT(textures.size() > textureHandel.id);
            return textures.at(textureHandel.id);
        }
    };
}
