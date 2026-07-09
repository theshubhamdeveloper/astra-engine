#pragma once

#include <astra/core/resource_manager.hpp>
#include <astra/graphics/batch.hpp>
#include <astra/graphics/camera.hpp>
#include <astra/graphics/mesh.hpp>
#include <astra/math/vector.hpp>
#include <astra/math/vertex.hpp>

namespace astra::graphics {
    constexpr uint32_t MAX_TEXTURE_SLOTS = 16;

    class Renderer {
        core::ResourceManager &resourceManager;
        const GraphicCamera &camera;

        Batch rectBatch;
        std::unique_ptr<Mesh> quadMesh;

        uint32_t drawCalls = 0;
        uint32_t previousDrawCalls = 0;

    public:
        explicit Renderer(core::ResourceManager &resourceManager, const GraphicCamera &camera);

        void generateQuadMesh();

        void initialize();

        void begin();

        void end();

        void draw(Batch &batch);

        void drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                      float rotation, const math::Color &color,
                      const math::Vec4 &cornerRadius, float strokeWidth, const math::Color &strokeColor,
                      const core::TextureHandle &texture);

        void drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                      float rotation, const math::Vec4 &cornerRadius, const math::Color &color, float strokeWidth,
                      const math::Color &strokeColor);

        void drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                      float rotation, const math::Vec4 &cornerRadius, float strokeWidth, const math::Color &strokeColor,
                      const core::TextureHandle &texture);

        void drawTextRect(const math::Vec2 &pos, const math::Vec2 &size, float rotation, const math::Color &color,
                          const math::Vec4 &cornerRadius, float strokeWidth, const math::Color &strokeColor,
                          const core::TextureHandle &texture, const core::AtlasRegion &uv);

        void drawText(const math::Vec2 &pos, const core::FontHandle &fontHandle, const std::string &text,
                      const math::Color &color);

        void drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color);

        void onWindowResize(const math::Vec2 &newSize);

        uint32_t getDrawCallsCount() const;
    };
}
