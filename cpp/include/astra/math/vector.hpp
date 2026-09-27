#pragma once

#include <cmath>
#include <concepts>

namespace astra::math {
    template<typename T>
    concept Arithmetic = std::integral<T> || std::floating_point<T>;

    template<Arithmetic T>
    struct Vector2 {
        T x{};
        T y{};

        // Constructors

        constexpr Vector2() = default;

        explicit constexpr Vector2(T value)
            : x(value), y(value) {
        }

        constexpr Vector2(T x, T y)
            : x(x), y(y) {
        }

        template<Arithmetic U>
        explicit constexpr Vector2(const Vector2<U> &other)
            : x(static_cast<T>(other.x)),
              y(static_cast<T>(other.y)) {
        }

        // Static

        [[nodiscard]]
        static constexpr Vector2 zero() {
            return {T{}, T{}};
        }

        [[nodiscard]]
        static constexpr Vector2 one() {
            return {T{1}, T{1}};
        }

        [[nodiscard]]
        static constexpr Vector2 up() {
            return {T{}, T{-1}};
        }

        [[nodiscard]]
        static constexpr Vector2 down() {
            return {T{}, T{1}};
        }

        [[nodiscard]]
        static constexpr Vector2 left() {
            return {T{-1}, T{}};
        }

        [[nodiscard]]
        static constexpr Vector2 right() {
            return {T{1}, T{}};
        }

        // Unary

        [[nodiscard]]
        constexpr Vector2 operator-() const {
            return {-x, -y};
        }

        // Vector Operators

        [[nodiscard]]
        constexpr Vector2 operator+(const Vector2 &other) const {
            return {x + other.x, y + other.y};
        }

        [[nodiscard]]
        constexpr Vector2 operator-(const Vector2 &other) const {
            return {x - other.x, y - other.y};
        }

        [[nodiscard]]
        constexpr Vector2 operator*(const Vector2 &other) const {
            return {x * other.x, y * other.y};
        }

        [[nodiscard]]
        constexpr Vector2 operator/(const Vector2 &other) const {
            return {x / other.x, y / other.y};
        }

        // Scalar Operators

        template<Arithmetic U>
        [[nodiscard]]
        constexpr Vector2 operator*(U scalar) const {
            return {
                x * static_cast<T>(scalar),
                y * static_cast<T>(scalar)
            };
        }

        template<Arithmetic U>
        [[nodiscard]]
        constexpr Vector2 operator/(U scalar) const {
            return {
                x / static_cast<T>(scalar),
                y / static_cast<T>(scalar)
            };
        }

        // Compound

        constexpr Vector2 &operator+=(const Vector2 &other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        constexpr Vector2 &operator-=(const Vector2 &other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        template<Arithmetic U>
        constexpr Vector2 &operator*=(U scalar) {
            x *= static_cast<T>(scalar);
            y *= static_cast<T>(scalar);
            return *this;
        }

        template<Arithmetic U>
        constexpr Vector2 &operator/=(U scalar) {
            x /= static_cast<T>(scalar);
            y /= static_cast<T>(scalar);
            return *this;
        }

        // Comparison

        [[nodiscard]]
        constexpr bool operator==(const Vector2 &) const = default;

        // Math

        [[nodiscard]]
        constexpr T lengthSquared() const {
            return x * x + y * y;
        }

        [[nodiscard]]
        T length() const
            requires std::floating_point<T> {
            return std::sqrt(lengthSquared());
        }

        [[nodiscard]]
        Vector2 normalized() const
            requires std::floating_point<T> {
            const T len = length();

            if (len == T{})
                return zero();

            return *this / len;
        }

        void normalize()
            requires std::floating_point<T> {
            const T len = length();

            if (len == T{})
                return;

            *this /= len;
        }

        [[nodiscard]]
        constexpr T dot(const Vector2 &other) const {
            return x * other.x + y * other.y;
        }

        [[nodiscard]]
        T distance(const Vector2 &other) const
            requires std::floating_point<T> {
            return (*this - other).length();
        }

        [[nodiscard]]
        constexpr T distanceSquared(const Vector2 &other) const {
            return (*this - other).lengthSquared();
        }
    };

    template<Arithmetic T, Arithmetic U>
    [[nodiscard]]
    constexpr Vector2<T> operator*(U scalar, const Vector2<T> &vec) {
        return vec * scalar;
    }

    template<Arithmetic T>
    struct Vector3 {
        T x{};
        T y{};
        T z{};

        // Constructors

        constexpr Vector3() = default;

        explicit constexpr Vector3(T value)
            : x(value), y(value), z(value) {
        }

        constexpr Vector3(T x, T y, T z)
            : x(x), y(y), z(z) {
        }

        constexpr Vector3(const Vector2<T> &xy, T z = {})
            : x(xy.x), y(xy.y), z(z) {
        }

        template<Arithmetic U>
        explicit constexpr Vector3(const Vector3<U> &other)
            : x(static_cast<T>(other.x)),
              y(static_cast<T>(other.y)),
              z(static_cast<T>(other.z)) {
        }

        template<Arithmetic U>
        explicit constexpr Vector3(const Vector2<U> &xy, T z = {})
            : x(static_cast<T>(xy.x)),
              y(static_cast<T>(xy.y)),
              z(z) {
        }

        // Static

        [[nodiscard]]
        static constexpr Vector3 zero() {
            return {T{}, T{}, T{}};
        }

        [[nodiscard]]
        static constexpr Vector3 one() {
            return {T{1}, T{1}, T{1}};
        }

        [[nodiscard]]
        static constexpr Vector3 up() {
            return {T{}, T{1}, T{}};
        }

        [[nodiscard]]
        static constexpr Vector3 down() {
            return {T{}, T{-1}, T{}};
        }

        [[nodiscard]]
        static constexpr Vector3 left() {
            return {T{-1}, T{}, T{}};
        }

        [[nodiscard]]
        static constexpr Vector3 right() {
            return {T{1}, T{}, T{}};
        }

        [[nodiscard]]
        static constexpr Vector3 forward() {
            return {T{}, T{}, T{-1}};
        }

        [[nodiscard]]
        static constexpr Vector3 backward() {
            return {T{}, T{}, T{1}};
        }

        // Unary

        [[nodiscard]]
        constexpr Vector3 operator-() const {
            return {-x, -y, -z};
        }

        // Vector Operators

        [[nodiscard]]
        constexpr Vector3 operator+(const Vector3 &other) const {
            return {x + other.x, y + other.y, z + other.z};
        }

        [[nodiscard]]
        constexpr Vector3 operator-(const Vector3 &other) const {
            return {x - other.x, y - other.y, z - other.z};
        }

        [[nodiscard]]
        constexpr Vector3 operator*(const Vector3 &other) const {
            return {x * other.x, y * other.y, z * other.z};
        }

        [[nodiscard]]
        constexpr Vector3 operator/(const Vector3 &other) const {
            return {x / other.x, y / other.y, z / other.z};
        }

        // Scalar Operators

        template<Arithmetic U>
        [[nodiscard]]
        constexpr Vector3 operator*(U scalar) const {
            return {
                x * static_cast<T>(scalar),
                y * static_cast<T>(scalar),
                z * static_cast<T>(scalar)
            };
        }

        template<Arithmetic U>
        [[nodiscard]]
        constexpr Vector3 operator/(U scalar) const {
            return {
                x / static_cast<T>(scalar),
                y / static_cast<T>(scalar),
                z / static_cast<T>(scalar)
            };
        }

        // Compound

        constexpr Vector3 &operator+=(const Vector3 &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr Vector3 &operator-=(const Vector3 &other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        template<Arithmetic U>
        constexpr Vector3 &operator*=(U scalar) {
            x *= static_cast<T>(scalar);
            y *= static_cast<T>(scalar);
            z *= static_cast<T>(scalar);
            return *this;
        }

        template<Arithmetic U>
        constexpr Vector3 &operator/=(U scalar) {
            x /= static_cast<T>(scalar);
            y /= static_cast<T>(scalar);
            z /= static_cast<T>(scalar);
            return *this;
        }

        // Comparison

        [[nodiscard]]
        constexpr bool operator==(const Vector3 &) const = default;

        // Math

        [[nodiscard]]
        constexpr T lengthSquared() const {
            return x * x + y * y + z * z;
        }

        [[nodiscard]]
        T length() const
            requires std::floating_point<T> {
            return std::sqrt(lengthSquared());
        }

        [[nodiscard]]
        Vector3 normalized() const
            requires std::floating_point<T> {
            const T len = length();

            if (len == T{})
                return zero();

            return *this / len;
        }

        void normalize()
            requires std::floating_point<T> {
            const T len = length();

            if (len == T{})
                return;

            *this /= len;
        }

        [[nodiscard]]
        constexpr T dot(const Vector3 &other) const {
            return x * other.x +
                   y * other.y +
                   z * other.z;
        }

        [[nodiscard]]
        constexpr Vector3 cross(const Vector3 &other) const {
            return {
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            };
        }

        [[nodiscard]]
        T distance(const Vector3 &other) const
            requires std::floating_point<T> {
            return (*this - other).length();
        }

        [[nodiscard]]
        constexpr T distanceSquared(const Vector3 &other) const {
            return (*this - other).lengthSquared();
        }
    };

    template<Arithmetic T, Arithmetic U>
    [[nodiscard]]
    constexpr Vector3<T> operator*(U scalar, const Vector3<T> &vec) {
        return vec * scalar;
    }

    template<Arithmetic T>
    struct Vector4 {
        T x{};
        T y{};
        T z{};
        T w{};

        // Constructors

        constexpr Vector4() = default;

        explicit constexpr Vector4(T value)
            : x(value), y(value), z(value), w(value) {
        }

        constexpr Vector4(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w) {
        }

        constexpr Vector4(
            const Vector2<T> &xy,
            T z = {},
            T w = {}
        )
            : x(xy.x), y(xy.y), z(z), w(w) {
        }

        constexpr Vector4(
            const Vector3<T> &xyz,
            T w = {}
        )
            : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {
        }

        template<Arithmetic U>
        explicit constexpr Vector4(const Vector4<U> &other)
            : x(static_cast<T>(other.x)),
              y(static_cast<T>(other.y)),
              z(static_cast<T>(other.z)),
              w(static_cast<T>(other.w)) {
        }

        template<Arithmetic U>
        explicit constexpr Vector4(
            const Vector2<U> &xy,
            T z = {},
            T w = {}
        )
            : x(static_cast<T>(xy.x)),
              y(static_cast<T>(xy.y)),
              z(z),
              w(w) {
        }

        template<Arithmetic U>
        explicit constexpr Vector4(
            const Vector3<U> &xyz,
            T w = {}
        )
            : x(static_cast<T>(xyz.x)),
              y(static_cast<T>(xyz.y)),
              z(static_cast<T>(xyz.z)),
              w(w) {
        }

        // Static

        [[nodiscard]]
        static constexpr Vector4 zero() {
            return {T{}, T{}, T{}, T{}};
        }

        [[nodiscard]]
        static constexpr Vector4 one() {
            return {T{1}, T{1}, T{1}, T{1}};
        }

        // Unary

        [[nodiscard]]
        constexpr Vector4 operator-() const {
            return {-x, -y, -z, -w};
        }

        // Vector Operators

        [[nodiscard]]
        constexpr Vector4 operator+(const Vector4 &other) const {
            return {
                x + other.x,
                y + other.y,
                z + other.z,
                w + other.w
            };
        }

        [[nodiscard]]
        constexpr Vector4 operator-(const Vector4 &other) const {
            return {
                x - other.x,
                y - other.y,
                z - other.z,
                w - other.w
            };
        }

        [[nodiscard]]
        constexpr Vector4 operator*(const Vector4 &other) const {
            return {
                x * other.x,
                y * other.y,
                z * other.z,
                w * other.w
            };
        }

        [[nodiscard]]
        constexpr Vector4 operator/(const Vector4 &other) const {
            return {
                x / other.x,
                y / other.y,
                z / other.z,
                w / other.w
            };
        }

        // Scalar Operators

        template<Arithmetic U>
        [[nodiscard]]
        constexpr Vector4 operator*(U scalar) const {
            return {
                x * static_cast<T>(scalar),
                y * static_cast<T>(scalar),
                z * static_cast<T>(scalar),
                w * static_cast<T>(scalar)
            };
        }

        template<Arithmetic U>
        [[nodiscard]]
        constexpr Vector4 operator/(U scalar) const {
            return {
                x / static_cast<T>(scalar),
                y / static_cast<T>(scalar),
                z / static_cast<T>(scalar),
                w / static_cast<T>(scalar)
            };
        }

        // Compound

        constexpr Vector4 &operator+=(const Vector4 &other) {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        constexpr Vector4 &operator-=(const Vector4 &other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        template<Arithmetic U>
        constexpr Vector4 &operator*=(U scalar) {
            x *= static_cast<T>(scalar);
            y *= static_cast<T>(scalar);
            z *= static_cast<T>(scalar);
            w *= static_cast<T>(scalar);
            return *this;
        }

        template<Arithmetic U>
        constexpr Vector4 &operator/=(U scalar) {
            x /= static_cast<T>(scalar);
            y /= static_cast<T>(scalar);
            z /= static_cast<T>(scalar);
            w /= static_cast<T>(scalar);
            return *this;
        }

        // Comparison

        [[nodiscard]]
        constexpr bool operator==(const Vector4 &) const = default;

        // Math

        [[nodiscard]]
        constexpr T lengthSquared() const {
            return x * x + y * y + z * z + w * w;
        }

        [[nodiscard]]
        T length() const
            requires std::floating_point<T> {
            return std::sqrt(lengthSquared());
        }

        [[nodiscard]]
        Vector4 normalized() const
            requires std::floating_point<T> {
            const T len = length();

            if (len == T{})
                return zero();

            return *this / len;
        }

        void normalize()
            requires std::floating_point<T> {
            const T len = length();

            if (len == T{})
                return;

            *this /= len;
        }

        [[nodiscard]]
        constexpr T dot(const Vector4 &other) const {
            return x * other.x +
                   y * other.y +
                   z * other.z +
                   w * other.w;
        }

        [[nodiscard]]
        T distance(const Vector4 &other) const
            requires std::floating_point<T> {
            return (*this - other).length();
        }

        [[nodiscard]]
        constexpr T distanceSquared(const Vector4 &other) const {
            return (*this - other).lengthSquared();
        }
    };

    template<Arithmetic T, Arithmetic U>
    [[nodiscard]]
    constexpr Vector4<T> operator*(U scalar, const Vector4<T> &vec) {
        return vec * scalar;
    }

    // Aliases
    using vec2 = Vector2<float>;
    using dvec2 = Vector2<double>;

    using ivec2 = Vector2<int32_t>;
    using uvec2 = Vector2<uint32_t>;

    using i64vec2 = Vector2<int64_t>;
    using u64vec2 = Vector2<uint64_t>;

    using vec3 = Vector3<float>;
    using dvec3 = Vector3<double>;

    using ivec3 = Vector3<int32_t>;
    using uvec3 = Vector3<uint32_t>;

    using i64vec3 = Vector3<int64_t>;
    using u64vec3 = Vector3<uint64_t>;

    using vec4 = Vector4<float>;
    using dvec4 = Vector4<double>;

    using ivec4 = Vector4<int32_t>;
    using uvec4 = Vector4<uint32_t>;

    using i64vec4 = Vector4<int64_t>;
    using u64vec4 = Vector4<uint64_t>;
}
