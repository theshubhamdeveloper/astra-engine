#pragma once
#include <vector>

#include <astra/math/color.hpp>
#include <astra/math/vector.hpp>

namespace astra::graphics {
    struct VertexLayout {
        uint32_t index;
        int size;
        uint32_t type;
        bool normalize;
        int stride;
        void *offset;
    };

    struct ShapeVertex {
        math::vec2 position;
        math::vec2 uv;
        math::vec2 size;
        math::vec4 cornerRadius;
        math::Color color;
        math::Color strokeColor;
        float strokeWidth = 0;
        float texId = 0.0f;

        static const std::array<VertexLayout, 8> &getLayout() {
            static const std::array<VertexLayout, 8> layout = {
                {
                    {
                        0, 2, GL_FLOAT, false, sizeof(ShapeVertex),
                        reinterpret_cast<void *>(offsetof(ShapeVertex, position))
                    },
                    {
                        1, 2, GL_FLOAT, false, sizeof(ShapeVertex),
                        reinterpret_cast<void *>(offsetof(ShapeVertex, uv))
                    },
                    {
                        2, 2, GL_FLOAT, false, sizeof(ShapeVertex),
                        reinterpret_cast<void *>(offsetof(ShapeVertex, size))
                    },
                    {
                        3, 4, GL_FLOAT, false, sizeof(ShapeVertex),
                        reinterpret_cast<void *>(offsetof(ShapeVertex, cornerRadius))
                    },
                    {
                        4, 4, GL_UNSIGNED_BYTE, true, sizeof(ShapeVertex),
                        reinterpret_cast<void *>(offsetof(ShapeVertex, color))
                    },
                    {
                        5, 4, GL_UNSIGNED_BYTE, true, sizeof(ShapeVertex),
                        reinterpret_cast<void *>(offsetof(ShapeVertex, strokeColor))
                    },
                    {
                        6, 1, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
                        reinterpret_cast<void *>(offsetof(ShapeVertex, strokeWidth))
                    },
                    {
                        7, 1, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
                        reinterpret_cast<void *>(offsetof(ShapeVertex, texId))
                    }
                }
            };
            return layout;
        }
    };

    struct TextVertex {
        math::vec2 position;
        math::vec2 uv;
        math::Color color;
        float texId = 0.0f;

        static const std::array<VertexLayout, 4> &getLayout() {
            static const std::array<VertexLayout, 4> layout = {
                {
                    {
                        0, 2, GL_FLOAT, false, sizeof(TextVertex),
                        reinterpret_cast<void *>(offsetof(TextVertex, position))
                    },
                    {
                        1, 2, GL_FLOAT, false, sizeof(TextVertex),
                        reinterpret_cast<void *>(offsetof(TextVertex, uv))
                    },
                    {
                        2, 4, GL_UNSIGNED_BYTE, true, sizeof(TextVertex),
                        reinterpret_cast<void *>(offsetof(TextVertex, color))
                    },
                    {
                        3, 1, GL_FLOAT, false, sizeof(TextVertex),
                        reinterpret_cast<void *>(offsetof(TextVertex, texId))
                    }
                }
            };
            return layout;
        }
    };
}
