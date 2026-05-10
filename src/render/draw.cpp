#include "render/draw.hpp"
#include "math/geometry.hpp"
using astra::math::Vec2;

namespace astra::render {
    void drawTriangle(Framebuffer &fb, const Vec2 &a, const Vec2 &b, const Vec2 &c, const math::Color &color) {
        const Vec2 min = {std::floor(std::min({a.x, b.x, c.x})), std::floor(std::min({a.y, b.y, c.y}))};
        const Vec2 max = {std::ceil(std::max({a.x, b.x, c.x})), std::ceil(std::max({a.y, b.y, c.y}))};

        for (auto y = static_cast<uint32_t>(min.y); y <= static_cast<uint32_t>(max.y); y++) {
            for (auto x = static_cast<uint32_t>(min.x); x <= static_cast<uint32_t>(max.x); x++) {
                int samplingsCovered = 0;

                // Sub-pixel sampling loop
                for (uint32_t sy = 1; sy <= superSamplingLevel; ++sy) {
                    for (uint32_t sx = 1; sx <= superSamplingLevel; ++sx) {
                        float px = static_cast<float>(x) + (static_cast<float>(sx) * subPixelStep);
                        float py = static_cast<float>(y) + (static_cast<float>(sy) * subPixelStep);

                        if (math::isPointInsideTriangle(a, b, c, {px, py})) {
                            samplingsCovered++;
                        }
                    }
                }

                if (samplingsCovered == 0) continue;

                math::Color samplingColor = color;
                samplingColor.a = static_cast<uint8_t>(
                    std::round((static_cast<float>(samplingsCovered) / totalSamples) * 255.0f)
                );

                fb.putPixel({static_cast<int32_t>(x), static_cast<int32_t>(y)}, samplingColor);
            }
        }
    }

    void drawTriangleFan(Framebuffer &fb, const std::vector<Vec2> &vertices, const math::Color color) {
        if (vertices.size() < 3)
            return;
        for (size_t i = 1; i < vertices.size() - 1; i++) {
            drawTriangle(fb, vertices[0], vertices[i], vertices[i + 1], color);
        }
    }

    void drawTriangleStrip(Framebuffer &fb, const std::vector<Vec2> &vertices, const math::Color color) {
        if (vertices.size() < 3)
            return;
        for (size_t i = 0; i < vertices.size() - 2; i++) {
            drawTriangle(fb, vertices[i], vertices[i + 1], vertices[i + 2], color);
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
