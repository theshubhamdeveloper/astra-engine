#pragma once

#include <astra/graphics/batch.hpp>
#include <astra/graphics/camera.hpp>
#include <astra/graphics/mesh.hpp>
#include <astra/math/vector.hpp>
#include <astra/math/vertex.hpp>

namespace astra::graphics {
    constexpr uint32_t MAX_TEXTURE_SLOTS = 16;

    class Renderer {
        ResourceManager &resourceManager;
        const GraphicCamera &camera;

        Batch rectBatch;
        std::unique_ptr<Mesh> quadMesh;

    public:
        explicit Renderer(ResourceManager &resourceManager, const GraphicCamera &camera);

        void generateQuadMesh();

        void initialize();

        void flush();

        void flush(Batch &batch) const;

        void checkCanFlush(Batch &batch) const;

        void drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                      float rotation, const math::Color &color,
                      const math::Vec4 &cornerRadius, const TextureHandel &texture);

        void drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                      float rotation, const math::Vec4 &cornerRadius, const math::Color &color);

        void drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                      float rotation, const math::Vec4 &cornerRadius, const TextureHandel &texture);

        void drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color);

        void onWindowResize(const math::Vec2 &newSize);
    };
}
