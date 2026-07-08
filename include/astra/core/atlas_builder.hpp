#pragma once
#include <vector>

#include <astra/assets/image.hpp>
#include <astra/math/vector.hpp>

namespace astra::core {
    struct Rect {
        math::Vec2 pos;
        math::Vec2 size;

        float right() const {
            return pos.x + size.x;
        }

        float bottom() const {
            return pos.y + size.y;
        }

        [[nodiscard]] bool intersects(const Rect &other) const {
            return !(right() <= other.pos.x ||
                     pos.x >= other.right() ||
                     bottom() <= other.pos.y ||
                     pos.y >= other.bottom());
        }

        [[nodiscard]] bool contains(const Rect &other) const {
            return (pos.x <= other.pos.x && right() >= other.right() &&
                    pos.y <= other.pos.y && bottom() >= other.bottom());
        }
    };

    struct AtlasRegion {
        float u0 = 0, v0 = 0;
        float u1 = 0, v1 = 0;
    };

    class AtlasBuilder {
        std::vector<Rect> freeRects;

    public:
        assets::Image atlas;

        explicit AtlasBuilder(const math::Vec2 &size, int colorChannels);

        AtlasRegion add(const assets::Image &image, uint32_t padding);

        int bestFreeIndex(const math::Vec2 &size) const;

        void spilt(const Rect &placedRect);

        void copyImage(const math::Vec2 &position, const assets::Image &image, uint32_t padding);
    };
}
