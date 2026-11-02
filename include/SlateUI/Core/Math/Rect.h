#pragma once
#include "Vec2.h"
#include <algorithm>

namespace slate
{
    class Rect
    {
    public:
        Vec2 position{0.f, 0.f};
        Vec2 size{0.f, 0.f};

        constexpr Rect(Vec2 _position, Vec2 _size)
        {
            position = _position;
            size = _size;
        }

        constexpr bool Contains(const Vec2 _point) const
        {
            Vec2 min = position;
            Vec2 max = position + size;

            return (
                (_point.x >= min.x && _point.x < max.x) &&
                (_point.y >= min.y && _point.y < max.y));
        }

        constexpr bool Intersects(const Rect &other) const
        {
            if (size.x <= 0.f || size.y <= 0.f)
                return false;
            if (other.size.x <= 0.f || other.size.y <= 0.f)
                return false;

            Vec2 min = position;
            Vec2 max = position + size;

            Vec2 other_min = other.position;
            Vec2 other_max = other.position + other.size;

            if (max.x <= other_min.x || other_max.x <= min.x)
                return false;
            if (max.y <= other_min.y || other_max.y <= min.y)
                return false;

            return true;
        }

        constexpr Rect Intersection(const Rect &other) const
        {
            Vec2 min = position;
            Vec2 max = position + size;

            Vec2 other_min = other.position;
            Vec2 other_max = other.position + other.size;

            float inter_x_min = std::max(min.x, other_min.x);
            float inter_y_min = std::max(min.y, other_min.y);
            float inter_x_max = std::min(max.x, other_max.x);
            float inter_y_max = std::min(max.y, other_max.y);

            if ((inter_x_max <= inter_x_min) == false||
                (inter_y_max <= inter_y_min) == false)
            {

                return Rect({inter_x_min, inter_y_min},
                            {inter_x_max - inter_x_min,
                             inter_y_max - inter_y_min});
            }

            return Rect{{0.f}, {0.f}};
        }
    };
}