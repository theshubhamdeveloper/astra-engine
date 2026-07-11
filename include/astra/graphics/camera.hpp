#pragma once

#include <astra/math/mat3.hpp>
#include <astra/math/vector.hpp>
#include "astra/core/types.hpp"

namespace astra::graphics {
    struct GraphicCamera {
        math::vec2 position;
        float rotation = 0.0f;
        float zoom = 1.0f;
        math::Mat3 projection;
                                                        
        [[nodiscard]] math::Mat3 getView() const {
            return math::Mat3::scale(zoom, zoom) *
                   math::Mat3::rotation(-rotation * core::RADIAN_CONVERSION_FACTOR) *
                   math::Mat3::translation(-position.x, -position.y);
        }

        constexpr static math::Mat3 orthographic(const float left, const float right, const float bottom,
                                                 const float top) {
            return {
                2.0f / (right - left), 0, -((right + left) / (right - left)),
                0, 2.0f / (top - bottom), -((top + bottom) / (top - bottom)),
                0, 0, 1
            };
        }
    };
}
