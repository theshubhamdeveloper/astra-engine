#include <glad/glad.h>
#include <astra/graphics/texture.hpp>
#include <astra/graphics/shader.hpp>

namespace astra::graphics {
    Texture::Texture(const assets::Image &image, const GLuint unit) : id(0), unit(unit) {
        glGenTextures(1, &id);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     image.pixels.data());

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::bindToUniform(const Shader &shader, const std::string &uniform) const {
        shader.use();
        use();
        glUniform1i(
            glGetUniformLocation(shader.getID(), uniform.c_str()),
            unit
        );
    }

    void Texture::use() const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }
}
