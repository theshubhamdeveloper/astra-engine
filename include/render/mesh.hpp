#pragma once
#include <vector>
#include <SDL3/SDL_opengl.h>

#include "math/vertex.hpp"

namespace astra::render {
    struct Mesh {
    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        int indexCount;

    public:
        Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices);

        ~Mesh();

        void addAttribute(GLuint layout, GLint size, GLenum type, GLsizei stride, const void *offset);

        void draw() const;

        void unbind() const;
    };
}
