#include "assets/image_loader.hpp"
#include "stb_image.h"
#include "core/assert.hpp"
#include <iostream>
#include "math/color.hpp"

namespace astra::assets::image_handler {
    Image load(const std::string &filePath, const bool verticalFlip) {
        stbi_set_flip_vertically_on_load(verticalFlip);

        Image image;

        unsigned char *data =
                stbi_load(filePath.c_str(),
                          &image.width,
                          &image.height,
                          &image.channels,
                          4);

        if (!data) {
            std::cerr << "STB Message:" << stbi_failure_reason() << std::endl;
            ASSERT(data);
        }

        image.pixels.insert(image.pixels.end(), data, data + (image.width * image.height * 4));

        stbi_image_free(data);
        return image;
    }
}
