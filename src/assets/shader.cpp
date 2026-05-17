#include <glad/glad.h>
#include <fstream>
#include "core/assert.hpp"
#include "assets/shader.hpp"

namespace astra::assets {
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

        return shader;
    }

    Shader::Shader(const std::string &vertexFilePath, const std::string &fragmentFilePath) {
        id = glCreateProgram();

        const GLuint vertexShader = generateShader(vertexFilePath, GL_VERTEX_SHADER);
        const GLuint fragmentShader = generateShader(fragmentFilePath, GL_FRAGMENT_SHADER);

        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);

        glLinkProgram(id);
    }

    Shader::~Shader() {
        glDeleteProgram(id);
    }

    void Shader::use() const {
        glUseProgram(id);
    }

    GLuint Shader::getID() const {
        return id;
    }
}
