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

    void Renderer::end() {
        draw();
        previousDrawCalls = drawCalls;
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

    void Renderer::drawRect(const Rect &rect) {
        prepareBatch(shapeShader, shapeMesh);

        const auto &[position, size, rotation, style] = rect;

        const auto model = math::Mat3::translation(position.x, position.y) *
                           math::Mat3::rotation(rotation * core::RADIAN_CONVERSION_FACTOR) *
                           math::Mat3::scale(size.x + (style.strokeWidth * 2), size.y + (style.strokeWidth * 2));

        const auto texSlot = static_cast<float>(batch.pushTexture(style.texture ? style.texture : whiteTexture));

        batch.pushVertex(ShapeVertex{
            model.transformPoint({-0.5f, 0.5f}), math::Vec2{0, 1}, size,
            style.cornerRadius, style.fill, style.strokeColor, style.strokeWidth, texSlot
        });
        batch.pushVertex(ShapeVertex{
            model.transformPoint({0.5f, 0.5f}), math::Vec2{1, 1}, size,
            style.cornerRadius, style.fill, style.strokeColor, style.strokeWidth, texSlot
        });
        batch.pushVertex(ShapeVertex{
            model.transformPoint({-0.5f, -0.5f}), math::Vec2{0, 0},
            size,
            style.cornerRadius, style.fill, style.strokeColor, style.strokeWidth, texSlot
        });
        batch.pushVertex(ShapeVertex{
            model.transformPoint({0.5f, -0.5f}), math::Vec2{1, 0}, size,
            style.cornerRadius, style.fill, style.strokeColor, style.strokeWidth, texSlot
        });
    }

    void Renderer::drawText(const Text &text) {
        const auto &[position, fontHandle, data, color, size] = text;

        Font &font = resourceManager.fonts.get(fontHandle);
        font.setSize(size);

        const core::TextureHandle &atlas = font.getAtlas();
        const int lineHeight = font.lineHeight();

        math::Vec2 pen = position;

        for (const auto &c: data) {
            const auto &[region, glyphSize, advance, bearing] = font.getGlyph(c);

            if (c == '\n') {
                pen.x = position.x;
                pen.y += lineHeight;
                continue;
            }

            const math::Vec2 glyphPos = {
                pen.x + bearing.x,
                pen.y - bearing.y
            };

            if (glyphSize != math::Vec2::zero()) {
                prepareBatch(textShader, textMesh);

                const auto model = math::Mat3::translation(glyphPos.x + glyphSize.x * 0.5f,
                                                           glyphPos.y + glyphSize.y * 0.5f) *
                                   math::Mat3::scale(glyphSize.x, glyphSize.y);

                const auto texSlot = static_cast<float>(batch.pushTexture(atlas));

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
        }
    }

    void Renderer::drawLine(const Line &line) {
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

    void Renderer::onWindowResize(const math::Vec2 &newSize) {
    }

    uint32_t Renderer::getDrawCallCount() const {
        return previousDrawCalls;
    }
}
