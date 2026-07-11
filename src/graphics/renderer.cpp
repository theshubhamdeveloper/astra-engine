#include <astra/core/types.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/graphics/texture.hpp>
#include <astra/math/vector.hpp>

namespace astra::graphics {
    Renderer::Renderer(core::ResourceManager &resourceManager,
                       const GraphicCamera &camera) : resourceManager(resourceManager), camera(camera) {
    }

    void Renderer::initialize(const math::vec2 &contentScale) {
        m_contentScale = contentScale;

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
        m_drawCalls = 0;
    }

    void Renderer::end() {
        draw();
        m_previousDrawCalls = m_drawCalls;
    }

    void Renderer::draw() {
        if (m_batch.getVertices().empty() || !m_batch.shader || !m_batch.mesh) return;

        const Mesh &mesh = resourceManager.meshes.get(m_batch.mesh);

        const Shader &shader = resourceManager.shaders.get(m_batch.shader);
        shader.use();

        std::vector<int> slots;
        m_batch.activateTextures(resourceManager, slots);

        shader.setUniformMat3f("uProjection", camera.projection);
        shader.setUniformMat3f("uView", camera.getView());
        shader.setUniform1iv("uTex", slots.size(), slots.data());

        mesh.setVertices(m_batch.getVertices());
        mesh.draw((m_batch.getVertexCount() / 4) * 6);

        m_batch.clear();
        m_drawCalls += 1;
    }

    void Renderer::drawRect(const Rect &rect) {
        prepareBatch(shapeShader, shapeMesh);

        const auto &[position, size, rotation, style] = rect;

        const math::vec2 sizeStrokeScale = (size + math::vec2{style.strokeWidth * 2}) * m_contentScale;

        const auto model = math::Mat3::translation((position.x + size.x * 0.5f) * m_contentScale.x,
                                                   (position.y + size.y * 0.5f) * m_contentScale.y) *
                           math::Mat3::rotation(rotation * core::RADIAN_CONVERSION_FACTOR) *
                           math::Mat3::scale(sizeStrokeScale.x, sizeStrokeScale.y);

        const auto texSlot = static_cast<float>(m_batch.pushTexture(style.texture ? style.texture : whiteTexture));

        m_batch.pushVertex(ShapeVertex{
            model.transformPoint({-0.5f, 0.5f}), math::vec2{0, 1}, size,
            style.cornerRadius, style.fill, style.strokeColor, style.strokeWidth, texSlot
        });
        m_batch.pushVertex(ShapeVertex{
            model.transformPoint({0.5f, 0.5f}), math::vec2{1, 1}, size,
            style.cornerRadius, style.fill, style.strokeColor, style.strokeWidth, texSlot
        });
        m_batch.pushVertex(ShapeVertex{
            model.transformPoint({-0.5f, -0.5f}), math::vec2{0, 0},
            size,
            style.cornerRadius, style.fill, style.strokeColor, style.strokeWidth, texSlot
        });
        m_batch.pushVertex(ShapeVertex{
            model.transformPoint({0.5f, -0.5f}), math::vec2{1, 0}, size,
            style.cornerRadius, style.fill, style.strokeColor, style.strokeWidth, texSlot
        });
    }

    void Renderer::drawText(const Text &text) {
        const auto &[position, fontHandle, data, color, size] = text;

        Font &font = resourceManager.fonts.get(fontHandle);
        font.setSize(size * m_contentScale.y);

        const core::TextureHandle &atlas = font.getAtlas();
        const int lineHeight = font.lineHeight();

        math::vec2 pen = position;

        for (const auto &c: data) {
            const auto &[region, glyphSize, advance, bearing] = font.getGlyph(c);

            if (c == '\n') {
                pen.x = position.x;
                pen.y += lineHeight;
                continue;
            }

            const math::vec2 glyphPos = {
                pen.x + bearing.x,
                pen.y - bearing.y
            };

            if (glyphSize != math::uvec2::zero()) {
                prepareBatch(textShader, textMesh);

                const auto model = math::Mat3::translation(glyphPos.x + glyphSize.x * 0.5f,
                                                           glyphPos.y + glyphSize.y * 0.5f) *
                                   math::Mat3::scale(glyphSize.x, glyphSize.y);

                const auto texSlot = static_cast<float>(m_batch.pushTexture(atlas));

                m_batch.pushVertex(TextVertex{
                    model.transformPoint({-0.5f, 0.5f}), math::vec2{region.u0, region.v1}, color, texSlot
                });
                m_batch.pushVertex(TextVertex{
                    model.transformPoint({0.5f, 0.5f}), math::vec2{region.u1, region.v1}, color, texSlot
                });
                m_batch.pushVertex(TextVertex{
                    model.transformPoint({-0.5f, -0.5f}), math::vec2{region.u0, region.v0}, color, texSlot
                });
                m_batch.pushVertex(TextVertex{
                    model.transformPoint({0.5f, -0.5f}), math::vec2{region.u1, region.v0}, color, texSlot
                });
            }

            pen.x += advance.x;
        }
    }

    void Renderer::drawLine(const Line &line) {
    }


    void Renderer::prepareBatch(const core::ShaderHandle &shader, const core::MeshHandle &mesh) {
        if (!m_batch.shader || !m_batch.mesh) {
            m_batch.shader = shader;
            m_batch.mesh = mesh;
            return;
        }

        if (m_batch.shader != shader || m_batch.mesh != mesh || m_batch.textureCount() == MAX_TEXTURE_SLOTS ||
            m_batch.getVertexCount() >= resourceManager.meshes.get(m_batch.mesh).getMaxVertices()) {
            draw();
            m_batch.shader = shader;
            m_batch.mesh = mesh;
        }
    }

    uint32_t Renderer::drawCallCount() const {
        return m_previousDrawCalls;
    }
}
