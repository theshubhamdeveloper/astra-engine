#pragma once

#include <vector>

#include <astra/graphics/material.hpp>
#include <astra/math/vertex.hpp>

namespace astra::graphics {
    struct Batch {
        std::vector<math::Vertex> vertices;
        Material material;
    };
}
