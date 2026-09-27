#pragma once
#include "astra/math/vector.hpp"

namespace astra::graphics {
    struct LineMatrics {
        float ascent;
        float descent;
        float leading;

        float baseline;

        math::vec2 bounding;
    };
}
