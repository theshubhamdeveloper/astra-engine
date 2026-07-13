#pragma once

#include <astra/math/vector.hpp>
#include <astra/core/atlas_builder.hpp>
#include <astra/core/resource_handles.hpp>

namespace astra::graphics {
    struct Glyph {
        core::TextureHandle atlas;
        core::AtlasRegion region;
        math::uvec2 size;
        math::ivec2 advance;
        math::ivec2 bearing;
    };
}
