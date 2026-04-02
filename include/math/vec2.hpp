#pragma once

namespace astra::math {
    struct Vec2 {
        double x, y;

        Vec2(double x, double y);

        void perpendicular();

        static Vec2 zero();

        static Vec2 one();

        static double dot(const Vec2 &a, const Vec2 &b);

        static double cross(const Vec2 &a, const Vec2 &b);

        Vec2 operator+(Vec2 b) const;

        Vec2 operator-(Vec2 b) const;

        Vec2 operator*(double scalar) const;

        Vec2 operator/(double scalar) const;

        bool operator==(Vec2 b) const;
    };
}
