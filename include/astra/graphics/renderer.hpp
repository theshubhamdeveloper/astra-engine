#pragma once

#include <astra/core/resource_manager.hpp>
#include <astra/graphics/batch.hpp>
#include <astra/graphics/camera.hpp>
#include <astra/graphics/mesh.hpp>
#include <astra/math/vector.hpp>
#include <astra/graphics/vertex.hpp>

namespace astra::graphics {
    constexpr uint32_t MAX_TEXTURE_SLOTS = 16;

    struct RectStyle {
        math::Color fill = math::Color::white();
        math::Vec4 cornerRadius = {};
        float strokeWidth = 0.0f;
        math::Color strokeColor = math::Color::white();
        core::TextureHandle texture = {};
    };

    struct Rect {
        math::Vec2 position = {};
        math::Vec2 size = math::Vec2::one();
        float rotation = 0.0f;
        RectStyle style;
    };

    struct Text {
        math::Vec2 position = {};
        core::FontHandle font = {};
        std::string_view text;
        math::Color color = math::Color::white();
        uint32_t size = 16.0f;
    };

    struct Line {
        math::Vec2 start = {};
        math::Vec2 end = {};
        math::Color color = math::Color::white();
        float thickness = 1.0f;
    };

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

        void draw();

        void drawRect(const Rect &rect);

        void drawText(const Text &text);

        void drawLine(const Line &line);

        void onWindowResize(const math::Vec2 &newSize);

        [[nodiscard]] uint32_t getDrawCallCount() const;

    private:
        void prepareBatch(const core::ShaderHandle &shader, const core::MeshHandle &mesh);

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
