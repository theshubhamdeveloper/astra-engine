#pragma once
#include <vector>

#include <astra/assets/image.hpp>
#include <astra/math/vector.hpp>

namespace astra::core {
    class AtlasBuilder {
        std::vector<math::Vec2> sizes;
        std::vector<math::Vec2> positions;

    public:
        assets::Image atlas;

        explicit AtlasBuilder(const math::Vec2 &size);

        void add(const assets::Image &image, uint32_t padding);
    };
}
