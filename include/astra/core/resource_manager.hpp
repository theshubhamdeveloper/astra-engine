#pragma once

#include <vector>

#include <astra/core/assert.hpp>
#include <astra/core/resource_handles.hpp>
#include <astra/graphics/shader.hpp>
#include <astra/graphics/texture.hpp>


namespace astra::core {
    class ResourceManager {
        std::vector<Shader> shaders;
        std::vector<Texture> textures;
        std::vector<graphics::Shader> shaders;
        std::vector<graphics::Texture> textures;
        std::string resourceRootPath;

    public:
        ResourceManager() : shaders(1), textures(1) {
        }

        void setResourceRootPath(const std::string &path) {
            resourceRootPath = path;
        }

        ShaderHandle loadShader(const std::string &vertFilePath, const std::string &fragFilePath) {
            shaders.emplace_back(resourceRootPath + vertFilePath, resourceRootPath + fragFilePath);
            return {static_cast<uint32_t>(shaders.size()) - 1};
        }

        [[nodiscard]] const graphics::Shader &getShader(const ShaderHandle &shaderHandle) const {
            ASSERT(shaders.size() > shaderHandle.id || shaderHandle.id > 0);
            return shaders.at(shaderHandle.id);
        }

        TextureHandle loadTexture(const std::string &filePath) {
            textures.emplace_back(assets::Image::load(resourceRootPath + filePath));
            return {static_cast<uint32_t>(textures.size()) - 1};
        }

        TextureHandle loadTexture(const assets::Image &image) {
            textures.emplace_back(image);
            return {static_cast<uint32_t>(textures.size()) - 1};
        }

        [[nodiscard]] const Texture &getTexture(const TextureHandel &textureHandel) const {
            ASSERT(shaders.size() > textureHandel.id || textureHandel.id > 0);
            return textures.at(textureHandel.id);
        }
    };
}
