#pragma once

#include <vector>

#include <astra/core/assert.hpp>
#include <astra/graphics/shader.hpp>

namespace astra::graphics {
    struct ShaderHandel {
        uint32_t id;
    };

    class ShaderManager {
        std::vector<Shader> shaders;

    public:
        ShaderHandel loadShader(const std::string &vertFilePath, const std::string &fragFilePath) {
            shaders.emplace_back(vertFilePath, fragFilePath);
            return {static_cast<uint32_t>(shaders.size()) - 1};
        }

        [[nodiscard]] const Shader &getShader(const ShaderHandel &shaderHandel) const {
            ASSERT(shaders.size() > shaderHandel.id);
            return shaders.at(shaderHandel.id);
        }
    };
}
