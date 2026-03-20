#pragma once

namespace astra {
    struct Vec2 {
        double x, y;

        Vec2(double x, double y);

        void perpendicular();

        static Vec2 zero();

        static Vec2 one();

        static double dot(Vec2 a, Vec2 b);

        Vec2 operator+(Vec2 b) const;

        Vec2 operator-(Vec2 b) const;

        Vec2 operator*(double scalar) const;

        Vec2 operator/(double scalar) const;

        bool operator==(Vec2 b) const;
    };
}
