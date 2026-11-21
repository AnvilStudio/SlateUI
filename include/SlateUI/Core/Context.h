#pragma once
#include "ElementHandle.h"
#include "Relationship.h"
#include "Layout/Layout.h"
#include <cstddef>
#include <cstdint>
#include <vector>

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

// Pool functions are the implementation boundary used by the Anvil adapter.
bool IsValid(ElementHandle handle, const ElementPool& pool);
ElementHandle CreateElement(ElementPool& pool);
bool DestroyElement(ElementHandle handle, ElementPool& pool);
bool AddChild(ElementHandle parent, ElementHandle child, ElementPool& pool);
bool RemoveChild(ElementHandle parent, ElementHandle child, ElementPool& pool);
bool Reparent(ElementHandle new_parent, ElementHandle child, ElementPool& pool);
void ResolveTree(ElementHandle root, const Rect& canvas, ElementPool& pool);

// Preserve the original Context-facing API used by SlateUI clients and tests.
inline bool IsValid(ElementHandle handle, const Context& ctx) {
    return IsValid(handle, ctx.pool);
}
inline ElementHandle CreateElement(Context& ctx) {
    return CreateElement(ctx.pool);
}
inline bool DestroyElement(ElementHandle& handle, Context& ctx) {
    if (!DestroyElement(ElementHandle{handle}, ctx.pool)) return false;
    handle = null_handle;
    return true;
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
}
