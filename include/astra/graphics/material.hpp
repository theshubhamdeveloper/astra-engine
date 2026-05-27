#pragma once

#include <astra/graphics/shader_manager.hpp>
#include <astra/graphics/texture_manager.hpp>

namespace astra::graphics {
    struct Material {
        ShaderHandel shader;
        TextureHandel texture;
    };
}
