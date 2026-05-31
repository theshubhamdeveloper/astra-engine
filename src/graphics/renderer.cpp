#include <astra/core/types.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/graphics/texture.hpp>
#include <astra/math/vector.hpp>


namespace astra::graphics {
    Renderer::Renderer(ResourceManager &resourceManager,
                       const GraphicCamera &camera) : resourceManager(resourceManager), camera(camera) {
    }

    void Renderer::generateQuadMesh() {
        std::vector<uint32_t> quadIndices;
        quadIndices.reserve((MAX_VERTICES / 4) * 6);

        for (int i = 0; i < MAX_VERTICES; i += 4) {
            quadIndices.push_back(i);
            quadIndices.push_back(i + 1);
            quadIndices.push_back(i + 2);
            quadIndices.push_back(i + 1);
            quadIndices.push_back(i + 2);
            quadIndices.push_back(i + 3);
        }

        quadMesh = std::make_unique<Mesh>(quadIndices);
        quadMesh->addAttribute(0,
                               2,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof(math::Vertex),
                               nullptr);
        quadMesh->addAttribute(1,
                               2,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof(math::Vertex),
                               reinterpret_cast<void *>(offsetof(math::Vertex, uv)));
        quadMesh->addAttribute(2,
                               4,
                               GL_UNSIGNED_BYTE,
                               GL_TRUE,
                               sizeof(math::Vertex),
                               reinterpret_cast<void *>(offsetof(math::Vertex, color)));
        quadMesh->addAttribute(3,
                               1,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof(math::Vertex),
                               reinterpret_cast<void *>(offsetof(math::Vertex, texId)));
        quadMesh->unbind();
    }

    void Renderer::initialize() {
        const ShaderHandel graphicsShader = resourceManager.loadShader("shaders/rect.vert",
                                                                       "shaders/rect.frag");

        rectBatch.material.shader = graphicsShader;
        rectBatch.addTextureInSlot(resourceManager.loadTexture(assets::Image{1, 1, 4, {255, 255, 255, 255}}));
        generateQuadMesh();
    }

    void Renderer::flush() {
        flush(rectBatch);
    }

    void Renderer::flush(Batch &batch) const {
        const Shader &shader = resourceManager.getShader(batch.material.shader);
        shader.use();

        const int texCount = batch.material.textures.size();
        std::vector<int> texSlots(texCount);
        for (int i = 0; i < texCount; ++i) {
            texSlots[i] = i;
            resourceManager.getTexture(batch.material.textures[i]).use(i);
        }

        shader.setUniformMat3f("uProjection", camera.projection);
        shader.setUniformMat3f("uView", camera.getView());
        shader.setUniform1iv("uTex", texCount, texSlots.data());

        quadMesh->addDynamicVertex(batch.vertices);
        quadMesh->draw((batch.vertices.size() / 4) * 6);

        batch.vertices.clear();
        batch.material.textures.erase(batch.material.textures.begin() + 1, batch.material.textures.end());
    }

    void Renderer::checkCanFlush(Batch &batch) const {
        if (batch.material.textures.size() == MAX_TEXTURE_SLOTS) {
            flush(batch);
            return;
        }

        if (batch.vertices.size() < MAX_VERTICES) {
            flush(batch);
        }
    }

    void Renderer::drawTriangle(const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                                const math::Color &color) {
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                            const float rotation, const math::Color &color, const TextureHandel &texture) {
        const auto model = math::Mat3::translation(pos.x, pos.y) *
                           math::Mat3::rotation(rotation * core::RADIAN_CONVERSION_FACTOR) *
                           math::Mat3::scale(size.x, size.y);

        const uint32_t texSlot = rectBatch.addTextureInSlot(texture);

        rectBatch.vertices.emplace_back(model.transformPoint({-0.5f, 0.5f}), math::Vec2{0, 1}, color,
                                        texSlot);
        rectBatch.vertices.emplace_back(model.transformPoint({0.5f, 0.5f}), math::Vec2{1, 1}, color,
                                        texSlot);
        rectBatch.vertices.emplace_back(model.transformPoint({-0.5f, -0.5f}), math::Vec2{0, 0}, color,
                                        texSlot);
        rectBatch.vertices.emplace_back(model.transformPoint({0.5f, -0.5f}), math::Vec2{1, 0}, color,
                                        texSlot);
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const float rotation,
                            const math::Color &color) {
        drawRect(pos, size, rotation, color, rectBatch.material.textures[0]);
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const float rotation,
                            const TextureHandel &texture) {
        drawRect(pos, size, rotation, math::Color::white(), texture);
    }

    void Renderer::drawCircle(const math::Vec2 &pos, const uint32_t r, const math::Color &color) {
    }

    void Renderer::drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color) {
    }

    void Renderer::onWindowResize(const math::Vec2 &newSize) {
    }
}
