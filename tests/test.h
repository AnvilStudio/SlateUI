#pragma once

#include "SlateUI/slate.h"

void tst_rect()
{

    constexpr slate::Rect a{{0.f, 0.f}, {100.f, 100.f}};
    constexpr slate::Rect b{{50.f, 50.f}, {100.f, 100.f}};
    constexpr slate::Rect c{{200.f, 200.f}, {50.f, 50.f}};

    static_assert(slate::Contains(a, {25.f, 25.f}));
    static_assert(!slate::Contains(a, {100.f, 25.f}));

    static_assert(slate::Intersects(a, b));
    static_assert(!slate::Intersects(a, c));

    static_assert(slate::Intersection(a, b).size.x == 50.f);
    static_assert(slate::Intersection(a, b).size.y == 50.f);

    static_assert(slate::Intersection(a, c).size.x == 0.f);
}

void test_layout()
{
    constexpr slate::Rect canvas{{0.f, 0.f}, {800.f, 600.f}};

    slate::Transform child = {
        .anchor = slate::Anchor::TOP_CENTER,
        .offset = {0.f},
        .size = {200.f, 100.f},
        .pivot = {0.5f, 0.5f}
    };

    slate::Rect NewRect = slate::ResolvePosition(canvas, child);

    std::cout << NewRect.position.x << ", " << NewRect.position.y << "\n";
}
