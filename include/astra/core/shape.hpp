#pragma once

#include <astra/math/vector.hpp>

namespace astra::core {
    struct Rect {
        math::uvec2 pos;
        math::uvec2 size;
        float rotation = 0.0f;

        [[nodiscard]] uint32_t left() const {
            return pos.x;
        }

        [[nodiscard]] uint32_t top() const {
            return pos.y;
        }

        [[nodiscard]] uint32_t right() const {
            return pos.x + size.x;
        }

        [[nodiscard]] uint32_t bottom() const {
            return pos.y + size.y;
        }

        [[nodiscard]] bool intersects(const Rect &other) const {
            return !(right() <= other.left() ||
                     left() >= other.right() ||
                     bottom() <= other.top() ||
                     top() >= other.bottom());
        }

        [[nodiscard]] bool contains(const Rect &other) const {
            return (left() <= other.left() && right() >= other.right() &&
                    top() <= other.top() && bottom() >= other.bottom());
        }
    };
}
