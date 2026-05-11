#pragma once
#include "image.hpp"
#include "render/framebuffer.hpp"

namespace astra::assets {
    struct Texture {
        int height;
        int width;
        render::Buffer pixels;

        [[nodiscard]] math::Color getPixel(const math::Point &position) const {
            return math::Color::unpackUint32(pixels[(position.y * width) + position.x]);
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
