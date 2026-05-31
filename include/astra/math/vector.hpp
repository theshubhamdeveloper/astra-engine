#pragma once

#include <cmath>

namespace astra::math {
    struct Vec2 {
        float x = 0.0f;
        float y = 0.0f;

        constexpr Vec2() = default;

        explicit constexpr Vec2(
            const float f
        ) : x(f), y(f) {
        }

        constexpr Vec2(const float x, const float y) : x(x), y(y) {
        }

        // Static
        static constexpr Vec2 zero() {
            return {0.0f, 0.0f};
        }

        static constexpr Vec2 one() {
            return {1.0f, 1.0f};
        }

        static constexpr Vec2 up() {
            return {0.0f, -1.0f};
        }

        static constexpr Vec2 down() {
            return {0.0f, 1.0f};
        }

        static constexpr Vec2 left() {
            return {-1.0f, 0.0f};
        }

        static constexpr Vec2 right() {
            return {1.0f, 0.0f};
        }

        // Operators
        constexpr Vec2 operator+(const Vec2 &other) const {
            return {x + other.x, y + other.y};
        }

        constexpr Vec2 operator-(const Vec2 &other) const {
            return {x - other.x, y - other.y};
        }

        constexpr Vec2 operator*(const Vec2 &other) const {
            return {x * other.x, y * other.y};
        }

        constexpr Vec2 operator/(const Vec2 &other) const {
            return {x / other.x, y / other.y};
        }

        constexpr Vec2 operator*(const float scalar) const {
            return {x * scalar, y * scalar};
        }

        constexpr Vec2 operator/(const float scalar) const {
            return {x / scalar, y / scalar};
        }

        Vec2 &operator+=(const Vec2 &other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vec2 &operator-=(const Vec2 &other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vec2 &operator*=(const float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vec2 &operator/=(const float scalar) {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        constexpr Vec2 operator-() const {
            return {-x, -y};
        }

        constexpr bool operator==(const Vec2 &other) const {
            return x == other.x && y == other.y;
        }

        constexpr bool operator!=(const Vec2 &other) const {
            return !(*this == other);
        }

        // Calculation
        [[nodiscard]] float length() const {
            return std::sqrt(x * x + y * y);
        }

        [[nodiscard]] constexpr float lengthSquared() const {
            return x * x + y * y;
        }

        [[nodiscard]] Vec2 normalized() const {
            const float len = length();

            if (len == 0.0f)
                return Vec2::zero();

            return {x / len, y / len};
        }

        void normalize() {
            const float len = length();

            if (len == 0.0f)
                return;

            x /= len;
            y /= len;
        }

        [[nodiscard]] constexpr float dot(const Vec2 &other) const {
            return x * other.x + y * other.y;
        }

        [[nodiscard]] float distance(const Vec2 &other) const {
            return (*this - other).length();
        }

        [[nodiscard]] constexpr float distanceSquared(const Vec2 &other) const {
            return (*this - other).lengthSquared();
        }
    };

    constexpr Vec2 operator*(const float scalar, const Vec2 &vec) {
        return {vec.x * scalar, vec.y * scalar};
    }

    struct Vec3 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        constexpr Vec3() = default;

        explicit constexpr Vec3(
            const float f
        ) : x(f), y(f), z(f) {
        }

        constexpr Vec3(const float x, const float y, const float z)
            : x(x), y(y), z(z) {
        }

        explicit constexpr Vec3(const Vec2 &vec, const float z = 0.0f)
            : x(vec.x), y(vec.y), z(z) {
        }

        // Static
        static constexpr Vec3 zero() {
            return {0.0f, 0.0f, 0.0f};
        }

        static constexpr Vec3 one() {
            return {1.0f, 1.0f, 1.0f};
        }

        static constexpr Vec3 up() {
            return {0.0f, 1.0f, 0.0f};
        }

        static constexpr Vec3 down() {
            return {0.0f, -1.0f, 0.0f};
        }

        static constexpr Vec3 left() {
            return {-1.0f, 0.0f, 0.0f};
        }

        static constexpr Vec3 right() {
            return {1.0f, 0.0f, 0.0f};
        }

        static constexpr Vec3 forward() {
            return {0.0f, 0.0f, -1.0f};
        }

        static constexpr Vec3 backward() {
            return {0.0f, 0.0f, 1.0f};
        }

        // Operators
        constexpr Vec3 operator+(const Vec3 &other) const {
            return {x + other.x, y + other.y, z + other.z};
        }

        constexpr Vec3 operator-(const Vec3 &other) const {
            return {x - other.x, y - other.y, z - other.z};
        }

        constexpr Vec3 operator*(const Vec3 &other) const {
            return {x * other.x, y * other.y, z * other.z};
        }

        constexpr Vec3 operator/(const Vec3 &other) const {
            return {x / other.x, y / other.y, z / other.z};
        }

        constexpr Vec3 operator*(const float scalar) const {
            return {x * scalar, y * scalar, z * scalar};
        }

        constexpr Vec3 operator/(const float scalar) const {
            return {x / scalar, y / scalar, z / scalar};
        }

        Vec3 &operator+=(const Vec3 &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vec3 &operator-=(const Vec3 &other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vec3 &operator*=(const float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vec3 &operator/=(const float scalar) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        constexpr Vec3 operator-() const {
            return {-x, -y, -z};
        }

        constexpr bool operator==(const Vec3 &other) const {
            return x == other.x &&
                   y == other.y &&
                   z == other.z;
        }

        constexpr bool operator!=(const Vec3 &other) const {
            return !(*this == other);
        }

        // Calculation
        [[nodiscard]] constexpr float length() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        [[nodiscard]] constexpr float lengthSquared() const {
            return x * x + y * y + z * z;
        }

        [[nodiscard]] constexpr Vec3 normalized() const {
            const float len = length();

            if (len == 0.0f)
                return zero();

            return {x / len, y / len, z / len};
        }

        constexpr void normalize() {
            const float len = length();

            if (len == 0.0f)
                return;

            x /= len;
            y /= len;
            z /= len;
        }

        [[nodiscard]] constexpr float dot(const Vec3 &other) const {
            return x * other.x +
                   y * other.y +
                   z * other.z;
        }

        [[nodiscard]] float distance(const Vec3 &other) const {
            return (*this - other).length();
        }

        [[nodiscard]] constexpr float distanceSquared(const Vec3 &other) const {
            return (*this - other).lengthSquared();
        }
    };

    constexpr Vec3 operator*(const float scalar, const Vec3 &vec) {
        return {
            vec.x * scalar,
            vec.y * scalar,
            vec.z * scalar
        };
    }

    struct Vec4 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;

        constexpr Vec4() = default;

        explicit constexpr Vec4(
            const float f
        ) : x(f), y(f), z(f), w(f) {
        }

        constexpr Vec4(
            const float x,
            const float y,
            const float z,
            const float w
        ) : x(x), y(y), z(z), w(w) {
        }

        explicit constexpr Vec4(
            const Vec2 &vec,
            const float z = 0.0f,
            const float w = 0.0f
        ) : x(vec.x), y(vec.y), z(z), w(w) {
        }

        explicit constexpr Vec4(
            const Vec3 &vec,
            const float w = 0.0f
        ) : x(vec.x), y(vec.y), z(vec.z), w(w) {
        }

        // Static
        static constexpr Vec4 zero() {
            return {0.0f, 0.0f, 0.0f, 0.0f};
        }

        static constexpr Vec4 one() {
            return {1.0f, 1.0f, 1.0f, 1.0f};
        }

        // Operators
        constexpr Vec4 operator+(const Vec4 &other) const {
            return {x + other.x, y + other.y, z + other.z, w + other.w};
        }

        constexpr Vec4 operator-(const Vec4 &other) const {
            return {x - other.x, y - other.y, z - other.z, w - other.w};
        }

        constexpr Vec4 operator*(const Vec4 &other) const {
            return {x * other.x, y * other.y, z * other.z, w * other.w};
        }

        constexpr Vec4 operator/(const Vec4 &other) const {
            return {x / other.x, y / other.y, z / other.z, w / other.w};
        }

        constexpr Vec4 operator*(const float scalar) const {
            return {
                x * scalar,
                y * scalar,
                z * scalar,
                w * scalar
            };
        }

        constexpr Vec4 operator/(const float scalar) const {
            return {
                x / scalar,
                y / scalar,
                z / scalar,
                w / scalar
            };
        }

        Vec4 &operator+=(const Vec4 &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        Vec4 &operator-=(const Vec4 &other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        Vec4 &operator*=(const float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        Vec4 &operator/=(const float scalar) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;
            return *this;
        }

        constexpr Vec4 operator-() const {
            return {-x, -y, -z, -w};
        }

        constexpr bool operator==(const Vec4 &other) const {
            return x == other.x &&
                   y == other.y &&
                   z == other.z &&
                   w == other.w;
        }

        constexpr bool operator!=(const Vec4 &other) const {
            return !(*this == other);
        }

        // Calculation
        [[nodiscard]] float length() const {
            return std::sqrt(
                x * x +
                y * y +
                z * z +
                w * w
            );
        }

        [[nodiscard]] constexpr float lengthSquared() const {
            return x * x +
                   y * y +
                   z * z +
                   w * w;
        }

        [[nodiscard]] Vec4 normalized() const {
            const float len = length();

            if (len == 0.0f)
                return zero();

            return {
                x / len,
                y / len,
                z / len,
                w / len
            };
        }

        void normalize() {
            const float len = length();

            if (len == 0.0f)
                return;

            x /= len;
            y /= len;
            z /= len;
            w /= len;
        }

        [[nodiscard]] constexpr float dot(const Vec4 &other) const {
            return x * other.x +
                   y * other.y +
                   z * other.z +
                   w * other.w;
        }

        [[nodiscard]] float distance(const Vec4 &other) const {
            return (*this - other).length();
        }

        [[nodiscard]] constexpr float distanceSquared(
            const Vec4 &other
        ) const {
            return (*this - other).lengthSquared();
        }
    };

    constexpr Vec4 operator*(
        const float scalar,
        const Vec4 &vec
    ) {
        return {
            vec.x * scalar,
            vec.y * scalar,
            vec.z * scalar,
            vec.w * scalar
        };
    }
}
