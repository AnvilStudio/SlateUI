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

            ElementSlot(Transform _transform, Rect _rect, Relationship _relation, uint32_t _generation, bool _alive)
                : transform(_transform), resolved(_rect), relationship(_relation), generation(_generation), alive(_alive)
            {
            }
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

    bool IsValid(ElementHandle _handle, ElementPool _pool)
    {
        if (_handle.index == -1 || _handle.generation == -1)
        {
            return false;
        }

        if (!_handle.index > _pool.slots.size())
        {
            if (_pool.slots[_handle.index].generation != _handle.generation)
            {
                return false;
            }

            return true;
        }

        return false;
    }

    ElementHandle CreateElement(ElementPool &_pool)
    {
        // use a prev freed slot;
        size_t free_size = _pool.free_indicies.size();
        if (free_size > 0)
        {
            uint32_t slot_index = _pool.free_indicies.back();
            _pool.free_indicies.pop_back();

            ElementSlot &slot = _pool.slots[slot_index];
            slot.alive = true;

            return ElementHandle{slot_index, ++slot.generation};
        }
        else
        {
            ElementSlot slot(Transform{}, Rect{{0.f, 0.f}, {0.f, 0.f}}, Relationship{}, 1, true);

            _pool.slots.push_back(slot);

            return ElementHandle{(_pool.slots.size() - 1), slot.generation};
        }
    }

    bool DestroyElement(ElementHandle _handle, ElementPool &_pool)
    {
        // reset the element slot
        if (IsValid(_handle, _pool))
        {
            auto &slot = _pool.slots[_handle.index];

            slot.transform = Transform{};
            slot.resolved = Rect{{0.f, 0.f}, {0.f, 0.f}};
            slot.relationship = Relationship{};
            slot.alive = false;

            _pool.free_indicies.push_back(_handle.index);

            _handle.index = -1;
            _handle.generation = -1;

            return true;
        }
        return false;
    }

    void AddChild(ElementHandle _parent, ElementHandle _child, ElementPool &_pool)
    {
    }

    void RemoveChild(ElementHandle _parent, ElementHandle _child, ElementPool &_pool)
    {
    }

    void Reparent(ElementHandle _new_parent, ElementHandle _child, ElementPool &_pool)
    {
    }

}
