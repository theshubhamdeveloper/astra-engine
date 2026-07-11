#pragma once

#include <astra/math/vector.hpp>
#include <astra/core/atlas_builder.hpp>

namespace astra::graphics {
    struct Glyph {
        core::AtlasRegion region;
        math::uvec2 size;
        math::uvec2 advance;
        math::uvec2 bearing;
    };
}
