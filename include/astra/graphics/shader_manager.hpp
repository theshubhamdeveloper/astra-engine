#pragma once

#include <unordered_map>

#include <astra/core/assert.hpp>
#include <astra/graphics/shader.hpp>

namespace astra::graphics {
    class ShaderManager {
        std::unordered_map<std::string, Shader> shaderMap;

    public:
        void loadShader(const std::string &name, const std::string &vertFilePath, const std::string &fragFilePath) {
            shaderMap.try_emplace(name, vertFilePath, fragFilePath);
        }

        [[nodiscard]] const Shader &getShader(const std::string &name) const {
            ASSERT(shaderMap.contains(name));
            return shaderMap.at(name);
        }
    };
}
