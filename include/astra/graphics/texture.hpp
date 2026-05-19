#pragma once

#include <glad/glad.h>
#include <astra/assets/image.hpp>
#include <astra/graphics/shader.hpp>

namespace astra::graphics {
    struct Texture {
    private:
        GLuint id;
        GLuint unit;

    public:
        Texture(const assets::Image &image, GLuint unit);

        void bindToUniform(const Shader &shader, const std::string &uniform) const;

        void use() const;
    };
}
