#include <SlateUI/Core/Context.h>

namespace slate
{
    bool IsValid(ElementHandle _handle, const Context &_ctx)
    {
        if (_handle.index == -1 || _handle.generation == -1)
        {
            return false;
        }

        if (!(_handle.index >= _ctx.pool.slots.size()))
        {
            if (_ctx.pool.slots[_handle.index].generation != _handle.generation)
            {
                return false;
            }
            else if (!(_ctx.pool.slots[_handle.index].alive))
            {
                return false;
            }
            return true;
        }

        return false;
    }

    ElementHandle CreateElement(Context &_ctx)
    {
        // use a prev freed slot;
        size_t free_size = _ctx.pool.free_indicies.size();
        if (free_size > 0)
        {
            uint32_t slot_index = _ctx.pool.free_indicies.back();
            _ctx.pool.free_indicies.pop_back();

            ElementSlot &slot = _ctx.pool.slots[slot_index];
            slot.alive = true;

            return ElementHandle{slot_index, ++slot.generation};
        }
        else
        {
            ElementSlot slot(Transform{}, Rect{{0.f, 0.f}, {0.f, 0.f}}, Relationship{}, 1, true);

            _ctx.pool.slots.push_back(slot);

            return ElementHandle{(_ctx.pool.slots.size() - 1), slot.generation};
        }
    }

    bool DestroyElement(ElementHandle &_handle, Context &_ctx)
    {
        // reset the element slot
        if (IsValid(_handle, _ctx))
        {
            auto &slot = _ctx.pool.slots[_handle.index];

            slot.transform = Transform{};
            slot.resolved = Rect{{0.f, 0.f}, {0.f, 0.f}};
            slot.relationship = Relationship{};
            slot.alive = false;

            _ctx.pool.free_indicies.push_back(_handle.index);

            _handle.index = -1;
            _handle.generation = -1;

            return true;
        }
        return false;
    }

    bool AddChild(ElementHandle _parent, ElementHandle _child, Context &_ctx)
    {
        // 1. Validate both handles.
        if (!IsValid(_parent, _ctx) || !IsValid(_child, _ctx))
            return false;

        // 2. An element cannot be its own parent.
        if (_parent == _child)
            return false;

        ElementSlot &parent_slot = _ctx.pool.slots[_parent.index];
        ElementSlot &child_slot = _ctx.pool.slots[_child.index];

        // 3. Reject elements that already have a parent.
        if (child_slot.relationship.parent != null_handle)
            return false;

        // 4. Check for circular relationships.
        ElementHandle current = _parent;

        while (current != null_handle)
        {
            // The proposed child is already an ancestor
            // of the proposed parent.
            if (current == _child)
                return false;

            if (!IsValid(current, _ctx))
                return false;

            current = _ctx.pool.slots[current.index].relationship.parent;
        }

        ElementHandle last = parent_slot.relationship.last_child;

        if (parent_slot.relationship.first_child != null_handle &&
            !IsValid(last, _ctx))
        {
            return false;
        }

        // 5. Establish the child's relationship.
        child_slot.relationship.parent = _parent;
        child_slot.relationship.next_sibling = null_handle;

        // 6. Append the child to the parent's children.
        if (parent_slot.relationship.first_child == null_handle)
        {
            // The parent has no children.
            parent_slot.relationship.first_child = _child;
            parent_slot.relationship.last_child = _child;
        }
        else
        {   
            // Link the previous last child to the new child.
            _ctx.pool.slots[last.index].relationship.next_sibling = _child;

            // The new child becomes the last child.
            parent_slot.relationship.last_child = _child;
        }

        return true;
    }

    bool RemoveChild(ElementHandle _parent, ElementHandle _child, Context &_ctx)
    {
        if (!IsValid(_parent, _ctx) || !IsValid(_child, _ctx))
            return false;
    }

    bool Reparent(ElementHandle _new_parent, ElementHandle _child, Context &_ctx)
    {
        if (!IsValid(_new_parent, _ctx) || !IsValid(_child, _ctx))
            return false;
    }
}
