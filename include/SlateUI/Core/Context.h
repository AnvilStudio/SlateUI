#pragma once
#include "ElementHandle.h"
#include "Relationship.h"
#include "Layout/Layout.h"
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

bool IsValid(ElementHandle handle, const ElementPool& pool);
ElementHandle CreateElement(ElementPool& pool);
bool DestroyElement(ElementHandle handle, ElementPool& pool);
bool AddChild(ElementHandle parent, ElementHandle child, ElementPool& pool);
bool RemoveChild(ElementHandle parent, ElementHandle child, ElementPool& pool);
bool Reparent(ElementHandle new_parent, ElementHandle child, ElementPool& pool);
void ResolveTree(ElementHandle root, const Rect& canvas, ElementPool& pool);
}