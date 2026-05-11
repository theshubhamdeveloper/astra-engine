#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "assets/image.hpp"

namespace astra::assets::image_handler {
    [[nodiscard]] Image load(const char *file);
}