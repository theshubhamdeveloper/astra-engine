#pragma once

#include <astra/graphics/resource_manager.hpp>

namespace astra::graphics {
    struct Material {
        ShaderHandel shader;
        std::vector<TextureHandel> textures;
    };
}
