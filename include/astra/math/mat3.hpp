#pragma once

#include <astra/math/vector.hpp>

namespace astra::math {
    struct Mat3 {
        float m[9] = {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1,
        };

        constexpr static Mat3 identity() {
            return {};
        }

        constexpr static Mat3 translation(const float x, const float y) {
            return {
                1, 0, x,
                0, 1, y,
                0, 0, 1
            };
        }

        constexpr static Mat3 scale(const float x, const float y) {
            return {
                x, 0, 0,
                0, y, 0,
                0, 0, 1
            };
        }

        static Mat3 rotation(const float radian) {
            const float c = std::cos(radian);
            const float s = std::sin(radian);
            return {
                c, -s, 0,
                s, c, 0,
                0, 0, 1
            };
        }

        [[nodiscard]] constexpr Vec2 transformPoint(const Vec2 &p) const {
            Vec3 result = (*this) * Vec3{p.x, p.y, 1.0f};
            return {result.x, result.y};
        }

        constexpr Mat3 operator*(const Mat3 &other) const {
            return {
                m[0] * other.m[0] + m[1] * other.m[3] + m[2] * other.m[6],
                m[0] * other.m[1] + m[1] * other.m[4] + m[2] * other.m[7],
                m[0] * other.m[2] + m[1] * other.m[5] + m[2] * other.m[8],

                m[3] * other.m[0] + m[4] * other.m[3] + m[5] * other.m[6],
                m[3] * other.m[1] + m[4] * other.m[4] + m[5] * other.m[7],
                m[3] * other.m[2] + m[4] * other.m[5] + m[5] * other.m[8],

                m[6] * other.m[0] + m[7] * other.m[3] + m[8] * other.m[6],
                m[6] * other.m[1] + m[7] * other.m[4] + m[8] * other.m[7],
                m[6] * other.m[2] + m[7] * other.m[5] + m[8] * other.m[8]
            };
        }

        constexpr Vec3 operator*(const Vec3 &other) const {
            return {
                m[0] * other.x + m[1] * other.y + m[2] * other.z,
                m[3] * other.x + m[4] * other.y + m[5] * other.z,
                other.z
            };
        }

        float &at(const size_t row, const size_t col) {
            return m[row * 3 + col];
        }

        [[nodiscard]] const float &at(const size_t row, const size_t col) const {
            return m[row * 3 + col];
        }
    };
}
