#include <astra/core/types.hpp>
#include <astra/graphics/renderer.hpp>
#include <astra/graphics/texture.hpp>
#include <astra/math/vector.hpp>


namespace astra::graphics {
    Renderer::Renderer() : colorBatch() {
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
        const ShaderHandel graphicsShader = resourceManager.loadShader("../Resources/assets/shaders/graphics_2d.vert",
                                                                       "../Resources/assets/shaders/graphics_2d.frag");


        colorBatch.material.shader = colorShader;
        colorBatch.material.shader = graphicsShader;

        generateQuadMesh();
    }

    void Renderer::flush(const GraphicCamera &camera) {
        const Shader &shader = resourceManager.getShader(colorBatch.material.shader);
        shader.use();
        shader.setUniformMat3f("uProjection", camera.projection);
        shader.setUniformMat3f("uView", camera.getView());

        quadMesh->addDynamicVertex(colorBatch.vertices);
        quadMesh->draw();

        colorBatch.vertices.clear();
    }

    void Renderer::drawTriangle(const math::Vec2 &a, const math::Vec2 &b, const math::Vec2 &c,
                                const math::Color &color) const {
    }

    void Renderer::drawRect(const math::Vec2 &pos, const math::Vec2 &size,
                            const float rotation, const math::Color &color) {
        const auto model = math::Mat3::translation(pos.x, pos.y) *
                           math::Mat3::rotation(rotation * core::RADIAN_CONVERSION_FACTOR) *
                           math::Mat3::scale(size.x, size.y);

        colorBatch.vertices.emplace_back(model.transformPoint({-0.5f, 0.5f}), math::Vec2{0, 1}, color);
        colorBatch.vertices.emplace_back(model.transformPoint({0.5f, 0.5f}), math::Vec2{1, 1}, color);
        colorBatch.vertices.emplace_back(model.transformPoint({-0.5f, -0.5f}), math::Vec2{0, 0}, color);
        colorBatch.vertices.emplace_back(model.transformPoint({0.5f, -0.5f}), math::Vec2{1, 0}, color);
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
