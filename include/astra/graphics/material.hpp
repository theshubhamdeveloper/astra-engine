#pragma once

#include <astra/core/resource_handles.hpp>

namespace astra::graphics {
    struct Material {
        core::ShaderHandle shader;
        std::vector<core::TextureHandle> textures;
    };
}
