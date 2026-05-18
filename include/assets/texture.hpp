#pragma once
#include <glad/glad.h>
#include "image.hpp"
#include "shader.hpp"

namespace astra::assets {
    struct Texture {
    private:
        GLuint id;
        GLuint unit;

    public:
        Texture(const Image &image, GLuint unit);

        void bindToUniform(const Shader &shader, const std::string &uniform) const;

        void use() const;
    };
}
