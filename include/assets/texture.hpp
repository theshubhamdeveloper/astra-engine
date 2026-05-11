#pragma once
#include "image.hpp"
#include "render/framebuffer.hpp"

namespace astra::assets {
    struct Texture {
        int height = 0;
        int width = 0;
        render::Buffer pixels;

        [[nodiscard]] math::Color interpolate(const math::Vec2 &uv, const math::Color &color) const {
            const math::Point position = {static_cast<int32_t>(uv.x * static_cast<float>(width - 1)),
                                          static_cast<int32_t>(uv.y * static_cast<float>(height - 1))};
            const math::Color texel = math::Color::unpackUint32(pixels[(position.y * width) + position.x]);

            return {static_cast<uint8_t>(color.r * texel.r / 255.0f),
                    static_cast<uint8_t>(color.g * texel.g / 255.0f),
                    static_cast<uint8_t>(color.b * texel.b / 255.0f)};
        }

        static Texture fromImage(const Image &image) {
            Texture texture;
            texture.height = image.height;
            texture.width = image.width;

            for (int i = 0; i < image.height * image.width * 4; i += 4) {
                texture.pixels.push_back(astra::math::Color{
                        image.pixels[i + 0],
                        image.pixels[i + 1],
                        image.pixels[i + 2],
                        image.pixels[i + 3]}.packRGBA());
            }

            return texture;
        }
    };

}
