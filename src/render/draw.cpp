#include "render/draw.hpp"
#include "assets/texture.hpp"
#include "math/geometry.hpp"
#include "math/vertex.hpp"
using astra::math::Vec2;

namespace astra::render {
    void drawTriangle(Framebuffer &fb, const math::Vertex &a, const math::Vertex &b, const math::Vertex &c,
                      const assets::Texture &texture) {
        const Vec2 min = {std::floor(std::min({a.position.x, b.position.x, c.position.x})),
                          std::floor(std::min({a.position.y, b.position.y, c.position.y}))};
        const Vec2 max = {std::ceil(std::max({a.position.x, b.position.x, c.position.x})),
                          std::ceil(std::max({a.position.y, b.position.y, c.position.y}))};

        for (auto y = static_cast<uint32_t>(min.y); y <= static_cast<uint32_t>(max.y); y++) {
            for (auto x = static_cast<uint32_t>(min.x); x <= static_cast<uint32_t>(max.x); x++) {

                // int samplingsCovered = 0;
                //
                // // Sub-pixel sampling loop
                // for (uint32_t sy = 1; sy <= superSamplingLevel; ++sy) {
                //     for (uint32_t sx = 1; sx <= superSamplingLevel; ++sx) {
                //         float px = static_cast<float>(x) + (static_cast<float>(sx) * subPixelStep);
                //         float py = static_cast<float>(y) + (static_cast<float>(sy) * subPixelStep);
                //
                //         if (math::isPointInsideTriangle(a.position, b.position, c.position, {px, py})) {
                //             samplingsCovered++;
                //         }
                //     }
                // }
                // if (samplingsCovered == 0) continue;

                const Vec2 position = {static_cast<float>(x), static_cast<float>(y)};

                if (!math::isPointInsideTriangle(a.position, b.position, c.position, position)) {
                    continue;
                }

                const float area = math::triangleEdge(a.position, b.position, c.position);
                const float u = math::triangleEdge(b.position, c.position, position) / area;
                const float v = math::triangleEdge(c.position, a.position, position) / area;
                const float w = math::triangleEdge(a.position, b.position, position) / area;

                const Vec2 uv = a.uv * u + b.uv * v + c.uv * w;
                const auto texColor = texture.getPixel({
                        static_cast<int32_t>(uv.x * static_cast<float>(texture.width - 1)),
                        static_cast<int32_t>(uv.y * static_cast<float>(texture.height - 1))});

                float interpR = ((a.color.r * u + b.color.r * v + c.color.r * w) / 255.0f) * (texColor.r / 255.0f);

                float interpG = ((a.color.g * u + b.color.g * v + c.color.g * w) / 255.0f) * (texColor.g / 255.0f);

                float interpB = ((a.color.b * u + b.color.b * v + c.color.b * w) / 255.0f) * (texColor.b / 255.0f);

                math::Color color = {
                        static_cast<uint8_t>(std::clamp(interpR, 0.0f, 1.0f) * 255),
                        static_cast<uint8_t>(std::clamp(interpG, 0.0f, 1.0f) * 255),
                        static_cast<uint8_t>(std::clamp(interpB, 0.0f, 1.0f) * 255)
                };

                fb.putPixel({static_cast<int32_t>(x), static_cast<int32_t>(y)}, color);
            }
        }
    }

    void drawTriangleFan(Framebuffer &fb, const std::vector<Vec2> &vertices, const math::Color color) {
        if (vertices.size() < 3)
            return;
        for (size_t i = 1; i < vertices.size() - 1; i++) {
            // drawTriangle(fb, vertices[0], vertices[i], vertices[i + 1], color);
        }
    }

    void drawTriangleStrip(Framebuffer &fb, const std::vector<Vec2> &vertices, const math::Color color) {
        if (vertices.size() < 3)
            return;
        for (size_t i = 0; i < vertices.size() - 2; i++) {
            // drawTriangle(fb, vertices[i], vertices[i + 1], vertices[i + 2], color);
        }
    }

    void drawRect(Framebuffer &fb, const Vec2 &pos, const Vec2 &size, const math::Color &color) {
        drawTriangleFan(fb,
                        {
                                pos,
                                {pos.x + size.x, pos.y},
                                {pos.x + size.x, pos.y + size.y},
                                {pos.x, pos.y + size.y},
                        },
                        color);
    }

    void drawCircle(Framebuffer &fb, const Vec2 &pos, const uint32_t r, const math::Color &color,
                    const uint32_t segment) {
        const std::vector<Vec2> points = math::generateCircleVertices(pos, r, segment);
        drawTriangleFan(fb, points, color);
    }

    void drawLine(Framebuffer &fb, const Vec2 &a, const Vec2 &b, const math::Color &color) {
        auto x0 = static_cast<int>(std::floor(a.x));
        auto y0 = static_cast<int>(std::floor(a.y));
        const auto x1 = static_cast<int>(std::floor(b.x));
        const auto y1 = static_cast<int>(std::floor(b.y));

        int stepX, stepY;

        int dx = x1 - x0;
        int dy = y1 - y0;

        if (dy < 0) {
            dy = -dy;
            stepY = -1;
        } else {
            stepY = 1;
        }
        if (dx < 0) {
            dx = -dx;
            stepX = -1;
        } else {
            stepX = 1;
        }
        dy <<= 1; /* dy is now 2*dy */
        dx <<= 1; /* dx is now 2*dx */

        fb.putPixel({x0, y0}, color);

        if (dx > dy) {
            int fraction = dy - (dx >> 1);
            while (x0 != x1) {
                x0 += stepX;
                if (fraction >= 0) {
                    y0 += stepY;
                    fraction -= dx;
                }
                fraction += dy;

                fb.putPixel({x0, y0}, color);
            }
        } else {
            int fraction = dx - (dy >> 1);
            while (y0 != y1) {
                if (fraction >= 0) {
                    x0 += stepX;
                    fraction -= dy;
                }
                y0 += stepY;
                fraction += dx;

                fb.putPixel({x0, y0}, color);
            }
        }
    }
}
