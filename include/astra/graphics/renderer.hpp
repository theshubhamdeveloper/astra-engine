#pragma once

#include <astra/graphics/batch.hpp>
#include <astra/graphics/camera.hpp>
#include <astra/graphics/mesh.hpp>
#include <astra/graphics/texture.hpp>
#include <astra/math/mat3.hpp>
#include <astra/math/vector.hpp>
#include <astra/math/vertex.hpp>

namespace astra::graphics {
    class Renderer {
        ResourceManager resourceManager;

        Batch colorBatch;
        std::unique_ptr<Mesh> quadMesh;

        math::Mat3 view;

    public:
        Renderer();

        void generateQuadMesh();

        void initialize();

        void flush(const GraphicCamera &camera);

        void drawTriangle(const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                          const math::Color &color) const;

        void drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                      float rotation, const math::Color &color);

        void drawRect(const math::Vec2 &pos, const math::Vec2 &size, const Texture &texture) const;

        void drawCircle(const math::Vec2 &pos, uint32_t r, const math::Color &color) const;

        void drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color) const;

        void onWindowResize(const math::Vec2 &newSize);
    };
}
