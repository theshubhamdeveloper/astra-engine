#pragma once

#include <astra/math/vector.hpp>
#include <astra/core/atlas_builder.hpp>

namespace astra::graphics {
    struct Glyph {
        core::AtlasRegion region;
        math::Vec2 size;
        math::Vec2 advance;
        math::Vec2 bearing;
    };
}
