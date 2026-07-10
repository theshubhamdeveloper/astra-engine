#pragma once

#include <astra/core/resource_manager.hpp>
#include <astra/graphics/batch.hpp>
#include <astra/graphics/camera.hpp>
#include <astra/graphics/mesh.hpp>
#include <astra/math/vector.hpp>
#include <astra/graphics/vertex.hpp>

namespace astra::graphics {
    constexpr uint32_t MAX_TEXTURE_SLOTS = 16;

    class Renderer {
        core::ResourceManager &resourceManager;
        const GraphicCamera &camera;
        core::TextureHandle whiteTexture;

        core::ShaderHandle shapeShader;
        core::ShaderHandle textShader;

        core::MeshHandle shapeMesh;
        core::MeshHandle textMesh;

        Batch batch;

        uint32_t drawCalls = 0;
        uint32_t previousDrawCalls = 0;

    public:
        explicit Renderer(core::ResourceManager &resourceManager, const GraphicCamera &camera);

        void initialize();

        void begin();

        void end();

        void prepareBatch(const core::ShaderHandle &shader, const core::MeshHandle &mesh);

        void draw();

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

        void drawText(const math::Vec2 &pos, const core::FontHandle &fontHandle, const std::string &text,
                      const math::Color &color);

        void drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color);

        void onWindowResize(const math::Vec2 &newSize);

        [[nodiscard]] uint32_t getDrawCallCount() const;

    private:
        template<class Vertex>
        core::MeshHandle createQuadMesh(const uint32_t maxVertices) {
            Mesh::Desc desc;
            desc.maxVertices = maxVertices;
            desc.indexBuffer.reserve((desc.maxVertices / 4) * 6);
            for (int i = 0; i < desc.maxVertices; i += 4) {
                desc.indexBuffer.push_back(i);
                desc.indexBuffer.push_back(i + 1);
                desc.indexBuffer.push_back(i + 2);
                desc.indexBuffer.push_back(i + 1);
                desc.indexBuffer.push_back(i + 2);
                desc.indexBuffer.push_back(i + 3);
            }
            desc.vertexSize = sizeof(Vertex);
            desc.layout.assign(Vertex::getLayout().begin(), Vertex::getLayout().end());
            return resourceManager.meshes.load(desc);
        }
    };
}
