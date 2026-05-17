#pragma once
#include <glad/glad.h>
#include <string>

namespace astra::assets {
    struct Shader {
    private:
        GLuint id;

        static std::string getFileContent(const std::string &filePath);

        static GLuint generateShader(const std::string &filePath, const GLenum &shaderType);

    public:
        Shader(const std::string &vertexFilePath, const std::string &fragmentFilePath);

        ~Shader();

        void use() const;

        [[nodiscard]] GLuint getID() const;
    };
}
