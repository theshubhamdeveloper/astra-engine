#include <fstream>

#include <glad/glad.h>

#include <astra/core/assert.hpp>
#include <astra/graphics/shader.hpp>

namespace astra::graphics {
    std::string Shader::getFileContent(const std::string &filePath) {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        ASSERT(file);

        return {
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        };
    }

    GLuint Shader::generateShader(const std::string &filePath, const GLenum &shaderType) {
        const GLuint shader = glCreateShader(shaderType);

        const std::string vertexShaderCode = getFileContent(filePath);

        glShaderSource(shader, 1, std::array{vertexShaderCode.data()}.data(), nullptr);
        glCompileShader(shader);

        GLint compileSuccess = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
        ASSERT(compileSuccess != false);

        return shader;
    }

    int Shader::getUniformLocation(const std::string &uniformName) const {
        const int location = glGetUniformLocation(id, uniformName.c_str());
        ASSERT(location != -1);
        return location;
    }

    Shader::Shader(const std::string &vertexFilePath, const std::string &fragmentFilePath) {
        id = glCreateProgram();

        const GLuint vertexShader = generateShader(vertexFilePath, GL_VERTEX_SHADER);
        const GLuint fragmentShader = generateShader(fragmentFilePath, GL_FRAGMENT_SHADER);

        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);

        GLint linkSuccess = 0;
        glGetProgramiv(id, GL_LINK_STATUS, &linkSuccess);
        ASSERT(linkSuccess);

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    }

    Shader::~Shader() {
        glDeleteProgram(id);
    }

    void Shader::use() const {
        glUseProgram(id);
    }

    void Shader::setUniform1i(const std::string &uniformName, const int value) const {
        glUniform1i(getUniformLocation(uniformName), value);
    }

    void Shader::setUniform1iv(const std::string &uniformName, const int count, const int *value) const {
        glUniform1iv(getUniformLocation(uniformName), count, value);
    }

    void Shader::setUniformVec2f(const std::string &uniformName, const math::Vec2 &value) const {
        glUniform2f(getUniformLocation(uniformName), value.x, value.y);
    }

    void Shader::setUniformVec3f(const std::string &uniformName, const math::Vec3 &value) const {
        glUniform3f(getUniformLocation(uniformName), value.x, value.y, value.z);
    }

    void Shader::setUniformMat3f(const std::string &uniformName, const math::Mat3 &value) const {
        glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_TRUE, value.m);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    GLuint Shader::getID() const {
        return id;
    }
}
