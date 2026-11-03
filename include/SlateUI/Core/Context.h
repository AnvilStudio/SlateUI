#pragma once
#include "ElementHandle.h"
#include "Relationship.h"
#include "ElementHandle.h"
#include "Layout/Layout.h"

#include <vector>

namespace slate
{
    namespace
    {
        struct ElementSlot
        {
            Transform transform;
            Rect resolved;
            Relationship relationship;

            uint32_t generation;
            bool alive;
        };

        struct ElementPool
        {
            std::vector<ElementSlot> slots;
            std::vector<uint32_t> free_indicies;
        };
    }

    struct Context
    {
        ElementPool pool;
    };

    ElementHandle CreateElement(ElementPool& _pool)
    {
    }

    bool DestroyElement(ElementHandle _handle, ElementPool& _pool)
    {
    }

    void AddChild(ElementHandle _parent, ElementHandle _child, ElementPool& _pool)
    {
    }

    void RemoveChild(ElementHandle _parent, ElementHandle _child, ElementPool& _pool)
    {
    }

    void Reparent(ElementHandle _new_parent, ElementHandle _child, ElementPool& _pool)
    {
    }

    bool IsValid(ElementHandle _handle, ElementPool _pool)
    {
    }
}
