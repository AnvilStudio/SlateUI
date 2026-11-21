#pragma once

#include "SlateUI/slate.h"
#include <cassert>

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

void tst_mem()
{
    // Create the context
    slate::Context ctx{};

    // Element Handles
    slate::ElementHandle A = slate::CreateElement(ctx);
    assert(slate::IsValid(A, ctx) && "Failed to create element A");
    slate::ElementHandle B = slate::CreateElement(ctx);
    assert(slate::IsValid(B, ctx) && "Failed to create element B");

    slate::DestroyElement(A, ctx);
    // try to destroy an invalid element
    if (!slate::DestroyElement(A, ctx))
    {
        std::cout << "Destroyed A Failed. Check Passed" << std::endl;
    }

    slate::ElementHandle C = slate::CreateElement(ctx);
    assert(slate::IsValid(C, ctx) && "Failed to create element C");

    if (slate::IsValid(A, ctx))
    {
        std::cout << "Element A was destroyed, but still returning as valid!" << std::endl;
    }
    if (!slate::IsValid(B, ctx))
    {
        std::cout << "Element B should return valid, but is not!" << std::endl;
    }
    if (!slate::IsValid(C, ctx))
    {
        std::cout << "Element C should return valid, but is not!" << std::endl;
    }
}

void test_parenting()
{
    std::cout << "\n=== Parenting ===" << std::endl;

    slate::Context ctx{};

    auto canvas = slate::CreateElement(ctx);
    auto childA = slate::CreateElement(ctx);
    auto childB = slate::CreateElement(ctx);
    auto childC = slate::CreateElement(ctx);

    assert(slate::AddChild(canvas, childA, ctx));
    assert(slate::AddChild(canvas, childB, ctx));
    assert(slate::AddChild(canvas, childC, ctx));

    std::vector<slate::ElementHandle> visited;

    slate::TraverseChildren(canvas, ctx, [&](slate::ElementHandle child)
                            { visited.push_back(child); });

    assert(visited.size() == 3);
    assert(visited[0] == childA);
    assert(visited[1] == childB);
    assert(visited[2] == childC);

    // Cannot parent an element to itself.
    assert(!slate::AddChild(canvas, canvas, ctx));

    // Cannot attach the same child twice.
    assert(!slate::AddChild(canvas, childA, ctx));

    // Cannot attach a child that already has a parent.
    assert(!slate::AddChild(childB, childA, ctx));

    // Cannot create a cycle.
    assert(!slate::AddChild(childA, canvas, ctx));

    auto nested = slate::CreateElement(ctx);

    assert(slate::AddChild(childA, nested, ctx));

    slate::debug::PrintTree(canvas, ctx);
}

void test_tree()
{
    std::cout << "\n=== Tree ===" << std::endl;

    slate::Context ctx{};

    auto canvas = slate::CreateElement(ctx);
    auto panelA = slate::CreateElement(ctx);
    auto panelB = slate::CreateElement(ctx);
    auto buttonA = slate::CreateElement(ctx);
    auto buttonB = slate::CreateElement(ctx);

    slate::AddChild(canvas, panelA, ctx);
    slate::AddChild(canvas, panelB, ctx);

    slate::AddChild(panelA, buttonA, ctx);
    slate::AddChild(panelA, buttonB, ctx);

    slate::debug::PrintTree(canvas, ctx);
}

void test_layout()
{
    constexpr slate::Rect canvas{{0.f, 0.f}, {800.f, 600.f}};

    slate::Transform child = {
        .anchor = slate::Anchor::TOP_CENTER,
        .offset = {0.f},
        .size = {200.f, 100.f},
        .pivot = {0.5f, 0.5f}};

    slate::Rect NewRect = slate::ResolvePosition(canvas, child);

    std::cout << NewRect.position.x << ", " << NewRect.position.y << "\n";
}
