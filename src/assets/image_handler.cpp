#include "assets/image_handler.hpp"
#include "stb_image.h"
#include "core/assert.hpp"
#include <iostream>

#include "math/color.hpp"


namespace astra::assets::image_handler {
    Image load(const char *file) {
        Image image;

        unsigned char *data =
                stbi_load(file,
                          &image.width,
                          &image.height,
                          &image.channels,
                          4);

        if (!data) {
            std::cerr << "STB Message:" << stbi_failure_reason() << std::endl;
            ASSERT(data);
        }

        for (int i = 0; i < image.width * image.height * 4; i++) {
            image.pixels.push_back(data[i]);
        }

        stbi_image_free(data);
        return image;
    }
}
