#pragma once

#include <string>

#include <glad/glad.h>

#include <astra/math/mat3.hpp>

namespace astra::graphics {
    struct Shader {
    private:
        uint32_t id = 0;

        static std::string getFileContent(const std::string &filePath);

        static uint32_t generateShader(const std::string &filePath, const GLenum &shaderType);

        [[nodiscard]] int getUniformLocation(const std::string &uniformName) const;

    public:
        Shader() = default;

        Shader(const std::string &vertexFilePath, const std::string &fragmentFilePath);

        ~Shader();

        void use() const;

        void setUniform1i(const std::string &uniformName, int value) const;

        void setUniformVec2f(const std::string &uniformName, const math::Vec2 &value) const;

        void setUniformVec3f(const std::string &uniformName, const math::Vec3 &value) const;

        void setUniformMat3f(const std::string &uniformName, const math::Mat3 &value) const;

        void unbind() const;

        [[nodiscard]] uint32_t getID() const;
    };
}
