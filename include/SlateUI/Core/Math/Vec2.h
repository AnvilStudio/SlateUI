#pragma once

namespace slate
{
    struct Vec2
    {
        float x, y;

        constexpr Vec2(float _x, float _y)
        : x(_x), y(_y)
        {
        }

        constexpr Vec2(float _initialize)
        : x(_initialize), y(_initialize)
        {
        }

        constexpr Vec2 operator+(const Vec2 &other) const
        {
            return {x + other.x, y + other.y};
        }

        constexpr Vec2 operator-(const Vec2 &other) const
        {
            return {x - other.x, y - other.y};
        }

        constexpr void operator=(const Vec2 &other)
        {
            this->x = other.x;
            this->y = other.y;
        }

        constexpr bool operator==(const Vec2 &other) const
        {
            return (
                (x == other.x) &&
                (y == other.y)
            );
        }
    };
}