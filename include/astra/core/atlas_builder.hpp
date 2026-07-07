#pragma once
#include <vector>

#include <astra/assets/image.hpp>
#include <astra/math/vector.hpp>

namespace astra::core {
    struct Rect {
        math::Vec2 pos;
        math::Vec2 size;

        [[nodiscard]] bool intersects(const Rect &other) const {
            return !(pos.x + size.x <= other.pos.x || pos.x >= other.pos.x + other.size.x ||
                     pos.y + size.y <= other.pos.y || pos.y >= other.pos.y + other.size.y);
        }

        [[nodiscard]] bool contains(const Rect &other) const {
            return (pos.x <= other.pos.x && pos.x + size.x >= other.pos.x + other.size.x &&
                    pos.y <= other.pos.y && pos.y + size.y >= other.pos.y + other.size.y);
        }
    };


    class AtlasBuilder {
        std::vector<Rect> freeRects;

    public:
        assets::Image atlas;

        explicit AtlasBuilder(const math::Vec2 &size);

        void add(const assets::Image &image, uint32_t padding);

        int bestFreeIndex(const math::Vec2 &size) const;

        void spilt(const Rect &placedRect);

        void copyImage(const math::Vec2 &position, const assets::Image &image, uint32_t padding);
    };
}
