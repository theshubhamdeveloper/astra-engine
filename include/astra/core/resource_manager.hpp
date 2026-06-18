#pragma once

#include <vector>

#include <astra/core/assert.hpp>
#include <astra/core/resource_handles.hpp>
#include <astra/graphics/font.hpp>
#include <astra/graphics/shader.hpp>
#include <astra/graphics/texture.hpp>


namespace astra::core {
    class ResourceManager {
        std::vector<graphics::Shader> shaders;
        std::vector<graphics::Texture> textures;
        std::vector<graphics::Font> fonts;
        std::string resourceRootPath;
        FT_Library ftLibrary;

    public:
        explicit ResourceManager(const FT_Library ftLibrary) : shaders(1), textures(1), ftLibrary(ftLibrary) {
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

        [[nodiscard]] const graphics::Texture &getTexture(const TextureHandle &textureHandle) const {
            ASSERT(textures.size() > textureHandle.id || textureHandle.id > 0);
            return textures.at(textureHandle.id);
        }

        FontHandle loadFont(const std::string &filePath) {
            fonts.emplace_back(*this, ftLibrary, resourceRootPath + filePath);
            return {static_cast<uint32_t>(fonts.size()) - 1};
        }

        [[nodiscard]] const graphics::Font &getFont(const FontHandle &fontHandle) const {
            ASSERT(fonts.size() > fontHandle.id || fontHandle.id > 0);
            return fonts.at(fontHandle.id);
        }

        [[nodiscard]] graphics::Font &getFont(const FontHandle &fontHandle) {
            ASSERT(fonts.size() > fontHandle.id || fontHandle.id > 0);
            return fonts.at(fontHandle.id);
        }
    };
}
