#pragma once

#include <vector>

#include <glad/glad.h>

#include <astra/math/vertex.hpp>

namespace astra::graphics {
    constexpr uint32_t MAX_VERTICES = 4000;

    struct Mesh {
    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        int indexCount;

    public:
        Mesh(const std::vector<GLuint> &indices);

        ~Mesh();

        void addAttribute(GLuint layout, GLint size, GLenum type, GLboolean normalize, GLsizei stride,
                          const void *offset);

        void addDynamicVertex(const std::vector<math::Vertex> &vertices) const;

        void draw() const;

        void unbind() const;
    };
}
