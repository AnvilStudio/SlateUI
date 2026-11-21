#include <SlateUI/slate.h>
#include <utility>

namespace slate {
bool IsValid(ElementHandle handle, const ElementPool& pool) {
    return handle.index < pool.slots.size() &&
           pool.slots[handle.index].alive &&
           pool.slots[handle.index].generation == handle.generation;
}

ElementHandle CreateElement(ElementPool& pool) {
    if (!pool.free_indices.empty()) {
        const size_t index = pool.free_indices.back();
        pool.free_indices.pop_back();
        auto& slot = pool.slots[index];
        const uint32_t generation = slot.generation;
        slot = ElementSlot{};
        slot.generation = generation;
        slot.alive = true;
        return {index, slot.generation};
    }
    pool.slots.emplace_back();
    auto& slot = pool.slots.back();
    slot.alive = true;
    return {pool.slots.size() - 1, slot.generation};
}

bool RemoveChild(ElementHandle parent, ElementHandle child, ElementPool& pool) {
    if (!IsValid(parent, pool) || !IsValid(child, pool) ||
        pool.slots[child.index].relationship.parent != parent) return false;
    auto& relation = pool.slots[parent.index].relationship;
    ElementHandle previous = null_handle;
    for (auto current = relation.first_child; IsValid(current, pool);
         current = pool.slots[current.index].relationship.next_sibling) {
        if (current == child) {
            const auto next = pool.slots[current.index].relationship.next_sibling;
            if (previous == null_handle) relation.first_child = next;
            else pool.slots[previous.index].relationship.next_sibling = next;
            if (relation.last_child == child) relation.last_child = previous;
            pool.slots[child.index].relationship.parent = null_handle;
            pool.slots[child.index].relationship.next_sibling = null_handle;
            return true;
        }
        previous = current;
    }
    return false;
}

bool AddChild(ElementHandle parent, ElementHandle child, ElementPool& pool) {
    if (!IsValid(parent, pool) || !IsValid(child, pool) || parent == child ||
        pool.slots[child.index].relationship.parent != null_handle) return false;
    for (auto ancestor = parent; IsValid(ancestor, pool);
         ancestor = pool.slots[ancestor.index].relationship.parent)
        if (ancestor == child) return false;
    auto& relation = pool.slots[parent.index].relationship;
    if (IsValid(relation.last_child, pool))
        pool.slots[relation.last_child.index].relationship.next_sibling = child;
    else relation.first_child = child;
    relation.last_child = child;
    pool.slots[child.index].relationship.parent = parent;
    pool.slots[child.index].relationship.next_sibling = null_handle;
    return true;
}

bool Reparent(ElementHandle new_parent, ElementHandle child, ElementPool& pool) {
    if (!IsValid(new_parent, pool) || !IsValid(child, pool) || new_parent == child)
        return false;
    for (auto ancestor = new_parent; IsValid(ancestor, pool);
         ancestor = pool.slots[ancestor.index].relationship.parent)
        if (ancestor == child) return false;
    const auto old = pool.slots[child.index].relationship.parent;
    if (IsValid(old, pool)) RemoveChild(old, child, pool);
    return AddChild(new_parent, child, pool);
}

bool DestroyElement(ElementHandle handle, ElementPool& pool) {
    if (!IsValid(handle, pool)) return false;
    // Destroy children before recycling this slot.
    while (IsValid(pool.slots[handle.index].relationship.first_child, pool))
        DestroyElement(pool.slots[handle.index].relationship.first_child, pool);
    const auto parent = pool.slots[handle.index].relationship.parent;
    if (IsValid(parent, pool)) RemoveChild(parent, handle, pool);
    auto& slot = pool.slots[handle.index];
    slot.transform = Transform{};
    slot.resolved = Rect{{0.f, 0.f}, {0.f, 0.f}};
    slot.relationship = Relationship{};
    slot.alive = false;
    ++slot.generation;
    if (slot.generation == 0) ++slot.generation;
    pool.free_indices.push_back(handle.index);
    return true;
}

void ResolveTree(ElementHandle root, const Rect& canvas, ElementPool& pool) {
    if (!IsValid(root, pool)) return;
    pool.slots[root.index].resolved = canvas;
    auto visit = [&](auto&& self, ElementHandle parent) -> void {
        const auto parentRect = pool.slots[parent.index].resolved;
        for (auto child = pool.slots[parent.index].relationship.first_child;
             IsValid(child, pool);
             child = pool.slots[child.index].relationship.next_sibling) {
            pool.slots[child.index].resolved =
                ResolvePosition(parentRect, pool.slots[child.index].transform);
            self(self, child);
        }
    };
    visit(visit, root);
}
}