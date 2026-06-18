#pragma once

#include <astra/core/resource_handles.hpp>
#include <astra/math/vector.hpp>

namespace astra::graphics {
    struct Glyph {
        core::TextureHandle texture;
        math::Vec2 size;
        math::Vec2 advance;
        math::Vec2 bearing;
    };
}
