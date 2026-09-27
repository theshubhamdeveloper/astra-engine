#pragma once

#include <vector>

#include <astra/assets/image.hpp>
#include <astra/core/shape.hpp>
#include <astra/math/vector.hpp>

namespace astra::core {
    struct AtlasRegion {
        float u0 = 0, v0 = 0;
        float u1 = 0, v1 = 0;
    };

    class AtlasBuilder {
        std::vector<Rect> freeRects;

    public:
        assets::Image atlas;

        explicit AtlasBuilder(const math::uvec2 &size, int colorChannels);

        AtlasRegion add(const assets::Image &image, uint32_t padding);

        [[nodiscard]] int bestFreeIndex(const math::uvec2 &size) const;

        void spilt(const Rect &placedRect);

        void copyImage(const math::uvec2 &position, const assets::Image &image, uint32_t padding);
    };
}
