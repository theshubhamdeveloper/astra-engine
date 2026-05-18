#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <vector>

namespace astra::assets {
    struct Image {
        int width;
        int height;
        int channels;
        std::vector<uint8_t> pixels;

        static Image load(const std::string &filePath, bool verticalFlip = true);
    };
}
