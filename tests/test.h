#pragma once

#include "SlateUI/slate.h"

void test_overlap_rect()
{

    // Overlapping rectangles
    static_assert(
        slate::Rect{{0.f, 0.f}, {100.f, 100.f}}
            .Intersection(slate::Rect{{50.f, 50.f}, {100.f, 100.f}})
            .size.x == 50.f);

    // Non-overlapping rectangles
    static_assert(
        slate::Rect{{0.f, 0.f}, {100.f, 100.f}}
            .Intersection(slate::Rect{{200.f, 200.f}, {50.f, 50.f}})
            .size.x == 0.f);

    // Rectangles touching at an edge
    static_assert(
        slate::Rect{{0.f, 0.f}, {100.f, 100.f}}
            .Intersection(slate::Rect{{100.f, 0.f}, {50.f, 50.f}})
            .size.x == 0.f);
}

constexpr bool TestAssignment()
{
    slate::Vec2 a{0.f, 0.f};
    slate::Vec2 b{50.f, 70.f};

    a = b;

    return a.x == 50.f && a.y == 70.f;
}
