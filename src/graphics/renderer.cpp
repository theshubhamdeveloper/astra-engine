#include <astra/core/types.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/graphics/texture.hpp>
#include <astra/math/vector.hpp>

namespace astra::graphics {
    Renderer::Renderer(core::ResourceManager &resourceManager,
                       const GraphicCamera &camera) : resourceManager(resourceManager), camera(camera) {
    }

    void Renderer::initialize() {
        shapeShader = resourceManager.shaders.load({
            "../Resources/shaders/shape.vert",
            "../Resources/shaders/shape.frag"
        });

        textShader = resourceManager.shaders.load({
            "../Resources/shaders/text.vert",
            "../Resources/shaders/text.frag"
        });

        shapeMesh = createQuadMesh<ShapeVertex>(20000);
        textMesh = createQuadMesh<TextVertex>(40000);

        whiteTexture = resourceManager.textures.load({assets::Image{1, 1, 4, {255, 255, 255, 255}}});
    }

    void Renderer::begin() {
        drawCalls = 0;
    }

    void Renderer::draw() {
        if (batch.getVertices().empty() || !batch.shader || !batch.mesh) return;

        const Mesh &mesh = resourceManager.meshes.get(batch.mesh);

        const Shader &shader = resourceManager.shaders.get(batch.shader);
        shader.use();

        std::vector<int> slots;
        batch.activateTextures(resourceManager, slots);

        shader.setUniformMat3f("uProjection", camera.projection);
        shader.setUniformMat3f("uView", camera.getView());
        shader.setUniform1iv("uTex", slots.size(), slots.data());

        mesh.setVertices(batch.getVertices());
        mesh.draw((batch.getVertexCount() / 4) * 6);

        batch.clear();
        drawCalls += 1;
    }

    void Renderer::end() {
        draw();
        previousDrawCalls = drawCalls;
    }

    void Renderer::prepareBatch(const core::ShaderHandle &shader, const core::MeshHandle &mesh) {
        if (!batch.shader || !batch.mesh) {
            batch.shader = shader;
            batch.mesh = mesh;
            return;
        }

        if (batch.shader != shader || batch.mesh != mesh || batch.textureCount() == MAX_TEXTURE_SLOTS ||
            batch.getVertexCount() >= resourceManager.meshes.get(batch.mesh).getMaxVertices()) {
            draw();
            batch.shader = shapeShader;
            batch.mesh = shapeMesh;
        }
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                            const float rotation, const math::Color &color, const math::Vec4 &cornerRadius,
                            const float strokeWidth, const math::Color &strokeColor,
                            const core::TextureHandle &texture) {
        prepareBatch(shapeShader, shapeMesh);

        // Padding 2px for aa
        const auto model = math::Mat3::translation(pos.x, pos.y) *
                           math::Mat3::rotation(rotation * core::RADIAN_CONVERSION_FACTOR) *
                           math::Mat3::scale(size.x + (strokeWidth * 2), size.y + (strokeWidth * 2));

        const float texSlot = batch.pushTexture(texture);

        batch.pushVertex(ShapeVertex{
            model.transformPoint({-0.5f, 0.5f}), math::Vec2{0, 1}, size,
            cornerRadius, color, strokeColor, strokeWidth, texSlot
        });
        batch.pushVertex(ShapeVertex{
            model.transformPoint({0.5f, 0.5f}), math::Vec2{1, 1}, size,
            cornerRadius, color, strokeColor, strokeWidth, texSlot
        });
        batch.pushVertex(ShapeVertex{
            model.transformPoint({-0.5f, -0.5f}), math::Vec2{0, 0},
            size,
            cornerRadius, color, strokeColor, strokeWidth, texSlot
        });
        batch.pushVertex(ShapeVertex{
            model.transformPoint({0.5f, -0.5f}), math::Vec2{1, 0}, size,
            cornerRadius, color, strokeColor, strokeWidth, texSlot
        });
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const float rotation,
                            const math::Vec4 &cornerRadius, const math::Color &color, const float strokeWidth,
                            const math::Color &strokeColor) {
        drawRect(pos, size, rotation, color, cornerRadius, strokeWidth, strokeColor, whiteTexture);
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const float rotation,
                            const math::Vec4 &cornerRadius, const float strokeWidth, const math::Color &strokeColor,
                            const core::TextureHandle &texture) {
        drawRect(pos, size, rotation, math::Color::white(), cornerRadius, strokeWidth, strokeColor, texture);
    }

    void Renderer::drawText(const math::Vec2 &pos, const core::FontHandle &fontHandle, const std::string &text,
                            const math::Color &color) {
        Font &font = resourceManager.fonts.get(fontHandle);
        const core::TextureHandle &fontAtlas = font.getAtlas();

        const int lineHeight = font.lineHeight();

        char previousChar = 0;

        math::Vec2 pen = pos;

        for (const auto &c: text) {
            const auto &[region, size, advance, bearing] = font.getGlyph(c);
            if (c == '\n') {
                pen.x = pos.x;
                pen.y += lineHeight;
                continue;
            }

            pen += font.getKerning(previousChar, c);

            const math::Vec2 glyphPos = {
                pen.x + bearing.x,
                pen.y - bearing.y
            };

            if (size != math::Vec2::zero()) {
                prepareBatch(textShader, textMesh);

                const auto model = math::Mat3::translation(glyphPos.x + size.x * 0.5f, glyphPos.y + size.y * 0.5f) *
                                   math::Mat3::scale(size.x, size.y);

                const auto texSlot = static_cast<float>(batch.pushTexture(fontAtlas));

                batch.pushVertex(TextVertex{
                    model.transformPoint({-0.5f, 0.5f}), math::Vec2{region.u0, region.v1}, color, texSlot
                });
                batch.pushVertex(TextVertex{
                    model.transformPoint({0.5f, 0.5f}), math::Vec2{region.u1, region.v1}, color, texSlot
                });
                batch.pushVertex(TextVertex{
                    model.transformPoint({-0.5f, -0.5f}), math::Vec2{region.u0, region.v0}, color, texSlot
                });
                batch.pushVertex(TextVertex{
                    model.transformPoint({0.5f, -0.5f}), math::Vec2{region.u1, region.v0}, color, texSlot
                });
            }

            pen.x += advance.x;
            previousChar = c;
        }
    }

    void Renderer::drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color) {
    }

    void Renderer::onWindowResize(const math::Vec2 &newSize) {
    }

    uint32_t Renderer::getDrawCallCount() const {
        return previousDrawCalls;
    }
}
