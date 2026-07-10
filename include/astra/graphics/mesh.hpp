#pragma once

#include <vector>

#include <glad/glad.h>

#include <astra/graphics/vertex.hpp>

namespace astra::graphics {
    struct Mesh {
        struct Desc {
            std::vector<uint32_t> indexBuffer;
            uint32_t maxVertices;
            uint32_t vertexSize;
            std::vector<VertexLayout> layout;
        };

        Mesh() : maxVertices(0), vertexSize(0) {
        }

        explicit Mesh(const Desc &desc);

        ~Mesh();

        void setVertices(const std::vector<uint8_t> &vertices) const;

        void draw(int elementCount) const;

        [[nodiscard]] uint32_t getMaxVertices() const;

    private:
        uint32_t vao = 0;
        uint32_t vbo = 0;
        uint32_t ebo = 0;

        uint32_t maxVertices;
        uint32_t vertexSize;
    };
}
