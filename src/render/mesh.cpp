#include <glad/glad.h>
#include "render/mesh.hpp"

namespace astra::render {
    Mesh::Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices) : VAO(0), VBO(0), EBO(0),
        indexCount(indices.size()) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(GLfloat),
            vertices.data(),
            GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indexCount * sizeof(GLuint),
            indices.data(),
            GL_STATIC_DRAW
        );
    }

    Mesh::~Mesh() {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &EBO);
    }

    void Mesh::addAttribute(const GLuint layout, const GLint size, const GLenum type, const GLsizei stride,
                            const void *offset) {
        glVertexAttribPointer(
            layout,
            size,
            type,
            GL_FALSE,
            stride,
            offset
        );
        glEnableVertexAttribArray(layout);
    }

    void Mesh::draw() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Mesh::unbind() const {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
