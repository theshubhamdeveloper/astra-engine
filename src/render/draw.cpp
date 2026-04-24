#include "render/draw.hpp"
#include "math/geometry.hpp"
using astra::math::Vec2;

namespace astra::render {
void drawTriangle(Framebuffer &fb, const Vec2 &a, const Vec2 &b, const Vec2 &c,
                  const math::Color &color) {
    const Vec2 min = {std::floor(std::min({a.x, b.x, c.x})),
                      std::floor(std::min({a.y, b.y, c.y}))};
    const Vec2 max = {std::ceil(std::max({a.x, b.x, c.x})),
                      std::ceil(std::max({a.y, b.y, c.y}))};

    for (auto y = static_cast<uint32_t>(min.y); y <= max.y; y++) {
        for (auto x = static_cast<uint32_t>(min.x); x <= max.x; x++) {
            if (math::isPointInsideTriangle(
                    a, b, c, {static_cast<double>(x), static_cast<double>(y)}))
                fb.putPixel(x, y, color);
        }
    }
}

void drawTriangleFan(Framebuffer &fb, const std::vector<Vec2> &vertices,
                     const math::Color color) {
    if (vertices.size() < 3)
        return;
    for (size_t i = 1; i < vertices.size() - 1; i++) {
        drawTriangle(fb, vertices[0], vertices[i], vertices[i + 1], color);
    }
}

void drawTriangleStrip(Framebuffer &fb, const std::vector<Vec2> &vertices,
                       const math::Color color) {
    if (vertices.size() < 3)
        return;
    for (size_t i = 0; i < vertices.size() - 2; i++) {
        drawTriangle(fb, vertices[i], vertices[i + 1], vertices[i + 2], color);
    }
}

void drawRect(Framebuffer &fb, const Vec2 &pos, const Vec2 &size,
              const math::Color &color) {
    drawTriangleFan(fb,
                    {
                        pos,
                        {pos.x + size.x, pos.y},
                        {pos.x + size.x, pos.y + size.y},
                        {pos.x, pos.y + size.y},
                    },
                    color);
}

void drawCircle(Framebuffer &fb, const Vec2 &pos, const uint32_t r,
                const math::Color &color, const uint32_t segment) {
    const std::vector<Vec2> points =
        math::generateCircleVertices(pos, r, segment);
    drawTriangleFan(fb, points, color);
}

void drawLine(Framebuffer &fb, const Vec2 &a, const Vec2 &b,
              const math::Color &color) {
    int x0 = std::floor(a.x);
    int y0 = std::floor(a.y);
    const int x1 = std::floor(b.x);
    const int y1 = std::floor(b.y);

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

    fb.putPixel(x0, y0, color);

    if (dx > dy) {
        int fraction = dy - (dx >> 1);
        while (x0 != x1) {
            x0 += stepX;
            if (fraction >= 0) {
                y0 += stepY;
                fraction -= dx;
            }
            fraction += dy;

            fb.putPixel(x0, y0, color);
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

            fb.putPixel(x0, y0, color);
        }
    }
}
}
