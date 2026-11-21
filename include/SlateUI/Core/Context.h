#pragma once
#include "ElementHandle.h"
#include "Relationship.h"
#include "Layout/Layout.h"
#include <cstdint>
#include <vector>
#include <iostream>

namespace slate {
struct ElementSlot {
    Transform transform{};
    Rect resolved{{0.f, 0.f}, {0.f, 0.f}};
    Relationship relationship{};
    uint32_t generation = 1;
    bool alive = false;
};

struct ElementPool {
    std::vector<ElementSlot> slots;
    std::vector<size_t> free_indices;
};

struct Context {
    ElementPool pool;
};

bool IsValid(ElementHandle handle, const ElementPool& pool);
ElementHandle CreateElement(ElementPool& pool);
bool DestroyElement(ElementHandle handle, ElementPool& pool);
bool AddChild(ElementHandle parent, ElementHandle child, ElementPool& pool);
bool RemoveChild(ElementHandle parent, ElementHandle child, ElementPool& pool);
bool Reparent(ElementHandle new_parent, ElementHandle child, ElementPool& pool);
void ResolveTree(ElementHandle root, const Rect& canvas, ElementPool& pool);

 
// Context is SlateUI's public ownership boundary. Retain the pool overloads
// for the Anvil adapter and forward Context calls to the same implementations.
inline bool IsValid(ElementHandle handle, const Context& ctx) {
    return IsValid(handle, ctx.pool);
}
inline ElementHandle CreateElement(Context& ctx) {
    return CreateElement(ctx.pool);
}
inline bool DestroyElement(ElementHandle handle, Context& ctx) {
    return DestroyElement(handle, ctx.pool);
}
inline bool AddChild(ElementHandle parent, ElementHandle child, Context& ctx) {
    return AddChild(parent, child, ctx.pool);
}
inline bool RemoveChild(ElementHandle parent, ElementHandle child, Context& ctx) {
    return RemoveChild(parent, child, ctx.pool);
}
inline bool Reparent(ElementHandle parent, ElementHandle child, Context& ctx) {
    return Reparent(parent, child, ctx.pool);
}
inline void ResolveTree(ElementHandle root, const Rect& canvas, Context& ctx) {
    ResolveTree(root, canvas, ctx.pool);
}

template<class Fn>
void TraverseChildren(ElementHandle parent, const Context& ctx, Fn&& fn) {
    if (!IsValid(parent, ctx)) return;
    for (auto child = ctx.pool.slots[parent.index].relationship.first_child;
         IsValid(child, ctx);
         child = ctx.pool.slots[child.index].relationship.next_sibling) {
        fn(child);
    }
}

namespace debug {
inline void PrintTree(ElementHandle root, const Context& ctx) {
    if (!IsValid(root, ctx)) return;
    auto visit = [&](auto&& self, ElementHandle handle, unsigned depth) -> void {
        if (!IsValid(handle, ctx)) return;
        for (unsigned n = 0; n < depth; ++n) std::cout << "  ";
        std::cout << "Element [" << handle.index << ":" << handle.generation << "]\n";
        TraverseChildren(handle, ctx, [&](ElementHandle child) {
            self(self, child, depth + 1);
        });
    };
    visit(visit, root, 0);
}
}
}
