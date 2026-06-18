#include <astra/core/types.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/graphics/texture.hpp>
#include <astra/math/vector.hpp>

namespace astra::graphics {
    Renderer::Renderer(core::ResourceManager &resourceManager,
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
                               2,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof(math::Vertex),
                               reinterpret_cast<void *>(offsetof(math::Vertex, size)));
        quadMesh->addAttribute(4,
                               4,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof(math::Vertex),
                               reinterpret_cast<void *>(offsetof(math::Vertex, cornerRadius)));
        quadMesh->addAttribute(5,
                               4,
                               GL_UNSIGNED_BYTE,
                               GL_TRUE,
                               sizeof(math::Vertex),
                               reinterpret_cast<void *>(offsetof(math::Vertex, strokeColor)));
        quadMesh->addAttribute(6,
                               1,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof(math::Vertex),
                               reinterpret_cast<void *>(offsetof(math::Vertex, strokeWidth)));
        quadMesh->addAttribute(7,
                               1,
                               GL_FLOAT,
                               GL_FALSE,
                               sizeof(math::Vertex),
                               reinterpret_cast<void *>(offsetof(math::Vertex, texId)));
        quadMesh->unbind();
    }

    void Renderer::initialize() {
        const core::ShaderHandle graphicsShader = resourceManager.loadShader("shaders/shape.vert",
                                                                             "shaders/shape.frag");

        rectBatch.material.shader = graphicsShader;
        rectBatch.addTextureInSlot(resourceManager.loadTexture(assets::Image{1, 1, 4, {255, 255, 255, 255}}));
        generateQuadMesh();
    }

    void Renderer::flush() {
        flush(rectBatch);
    }

    void Renderer::flush(Batch &batch) const {
        if (batch.vertices.empty()) return;
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
        if (batch.material.textures.size() >= MAX_TEXTURE_SLOTS) {
            flush(batch);
            return;
        }

        if (batch.vertices.size() >= MAX_VERTICES) {
            flush(batch);
        }
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                            const float rotation, const math::Color &color, const math::Vec4 &cornerRadius,
                            const float strokeWidth, const math::Color &strokeColor,
                            const core::TextureHandle &texture) {
        // Padding 2px for aa
        const auto model = math::Mat3::translation(pos.x, pos.y) *
                           math::Mat3::rotation(rotation * core::RADIAN_CONVERSION_FACTOR) *
                           math::Mat3::scale(size.x + (strokeWidth * 2), size.y + (strokeWidth * 2));

        const uint32_t texSlot = rectBatch.addTextureInSlot(texture);

        rectBatch.vertices.emplace_back(model.transformPoint({-0.5f, 0.5f}), math::Vec2{0, 1}, color,
                                        size, cornerRadius, strokeWidth, strokeColor, texSlot);
        rectBatch.vertices.emplace_back(model.transformPoint({0.5f, 0.5f}), math::Vec2{1, 1}, color,
                                        size, cornerRadius, strokeWidth, strokeColor, texSlot);
        rectBatch.vertices.emplace_back(model.transformPoint({-0.5f, -0.5f}), math::Vec2{0, 0}, color,
                                        size, cornerRadius, strokeWidth, strokeColor, texSlot);
        rectBatch.vertices.emplace_back(model.transformPoint({0.5f, -0.5f}), math::Vec2{1, 0}, color,
                                        size, cornerRadius, strokeWidth, strokeColor, texSlot);
        checkCanFlush(rectBatch);
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const float rotation,
                            const math::Vec4 &cornerRadius, const math::Color &color, const float strokeWidth,
                            const math::Color &strokeColor) {
        drawRect(pos, size, rotation, color, cornerRadius, strokeWidth, strokeColor, rectBatch.material.textures[0]);
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const float rotation,
                            const math::Vec4 &cornerRadius, const float strokeWidth, const math::Color &strokeColor,
                            const core::TextureHandle &texture) {
        drawRect(pos, size, rotation, math::Color::white(), cornerRadius, strokeWidth, strokeColor, texture);
    }

    void Renderer::drawText(const math::Vec2 &pos, const core::FontHandle &font, const std::string &text,
                            const math::Color &color) {
        math::Vec2 pen = pos;
        for (const auto &textChar: text) {
            const auto &[texture, size, advance, bearing] = resourceManager.getFont(font).getGlyph(textChar);
            math::Vec2 glyphPos{
                pen.x + bearing.x + size.x * 0.5f,
                pen.y - bearing.y + size.y * 0.5f
            };
            if (size != math::Vec2::zero())
                drawRect(glyphPos,
                         size, 0, color, math::Vec4{0}, 0,
                         math::Color::transparent(),
                         texture);

            pen.x += advance.x;
        }
    }

    void Renderer::drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color) {
    }

    void Renderer::onWindowResize(const math::Vec2 &newSize) {
    }
}
