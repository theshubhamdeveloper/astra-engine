#include <glad/glad.h>

#include <astra/graphics/mesh.hpp>

namespace astra::graphics {
    Mesh::Mesh(const std::vector<GLuint> &indices) : VAO(0), VBO(0), EBO(0),
                                                     indexCount(indices.size()) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            MAX_VERTICES * sizeof(math::Vertex),
            nullptr,
            GL_DYNAMIC_DRAW
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

    void Mesh::addAttribute(const GLuint layout, const GLint size, const GLenum type, GLboolean normalize,
                            const GLsizei stride,
                            const void *offset) {
        glBindVertexArray(VAO);

        glVertexAttribPointer(
            layout,
            size,
            type,
            normalize,
            stride,
            offset
        );
        glEnableVertexAttribArray(layout);
    }

    void Mesh::addDynamicVertex(const std::vector<math::Vertex> &vertices) const {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            vertices.size() * sizeof(math::Vertex),
            vertices.data()
        );

        glBindBuffer(GL_ARRAY_BUFFER, 0);
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
