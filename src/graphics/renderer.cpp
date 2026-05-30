#include <astra/core/types.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/graphics/texture.hpp>
#include <astra/math/vector.hpp>


namespace astra::graphics {
    Renderer::Renderer(ResourceManager &resourceManager) : resourceManager(resourceManager), batch() {
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
                               4,
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
        quadMesh->unbind();
    }

    void Renderer::initialize() {
        const ShaderHandel graphicsShader = resourceManager.loadShader("shaders/graphics_2d.vert",
                                                                       "shaders/graphics_2d.frag");

        batch.material.shader = graphicsShader;
        // batch.material.texture = resourceManager.loadTexture("images/texture.png");

        generateQuadMesh();
    }

    void Renderer::flush(const GraphicCamera &camera) {
        const Shader &shader = resourceManager.getShader(batch.material.shader);
        shader.use();

        // resourceManager.getTexture(batch.material.texture).use(0);

        shader.setUniformMat3f("uProjection", camera.projection);
        shader.setUniformMat3f("uView", camera.getView());
        // shader.setUniform1i("uUseTex", 1);
        // shader.setUniform1i("uTex", 0);

        quadMesh->addDynamicVertex(batch.vertices);
        quadMesh->draw();

        batch.vertices.clear();
    }

    void Renderer::drawTriangle(const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                                const math::Color &color) const {
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                            const float rotation, const math::Color &color) {
        const auto model = math::Mat3::translation(pos.x, pos.y) *
                           math::Mat3::rotation(rotation * core::RADIAN_CONVERSION_FACTOR) *
                           math::Mat3::scale(size.x, size.y);

        batch.vertices.emplace_back(model.transformPoint({-0.5f, 0.5f}), math::Vec2{0, 1}, color);
        batch.vertices.emplace_back(model.transformPoint({0.5f, 0.5f}), math::Vec2{1, 1}, color);
        batch.vertices.emplace_back(model.transformPoint({-0.5f, -0.5f}), math::Vec2{0, 0}, color);
        batch.vertices.emplace_back(model.transformPoint({0.5f, -0.5f}), math::Vec2{1, 0}, color);
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size, const Texture &texture) const {
    }

    void Renderer::drawCircle(const math::Vec2 &pos, const uint32_t r, const math::Color &color) const {
    }

    void Renderer::drawLine(const math::Vec2 &a, const math::Vec2 &b, const math::Color &color) const {
    }

    void Renderer::onWindowResize(const math::Vec2 &newSize) {
    }
}
