#include <glad/glad.h>
#include <astra/graphics/mesh.hpp>

namespace astra::graphics {
    Mesh::Mesh(const Desc &desc) : maxVertices(desc.maxVertices), vertexSize(desc.vertexSize) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(
            GL_ARRAY_BUFFER,
            desc.maxVertices * desc.vertexSize,
            nullptr,
            GL_DYNAMIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            desc.indexBuffer.size() * sizeof(GLuint),
            desc.indexBuffer.data(),
            GL_STATIC_DRAW
        );

        for (auto [layout, size, type, normalize, stride, offset]: desc.layout) {
            glVertexAttribPointer(
                layout,
                size,
                type,
                normalize,
                stride,
                offset);
            glEnableVertexAttribArray(layout);
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Mesh::setVertices(const std::vector<uint8_t> &vertices) const {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            vertices.size() * sizeof(uint8_t),
            vertices.data()
        );

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Mesh::draw(const int elementCount) const {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    Mesh::~Mesh() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &ebo);
    }

    uint32_t Mesh::getMaxVertices() const {
        return maxVertices;
    }
}
