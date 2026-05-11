#pragma once
#include <vector>

namespace astra::assets {
    struct Image {
        int width;
        int height;
        int channels;

        std::vector<uint8_t> pixels;
    };
}