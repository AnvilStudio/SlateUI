#pragma once
#include "../Math/Vec2.h"
#include "../Math/Rect.h"

namespace slate
{
    enum class Anchor
    {
        TOP_LEFT,
        CENTER_LEFT,
        BOTTOM_LEFT,

        TOP_CENTER,
        CENTER_CENTER,
        BOTTOM_CENTER,

        TOP_RIGHT,
        CENTER_RIGHT,
        BOTTOM_RIGHT
    };

    struct Transform
    {
        Anchor anchor = Anchor::CENTER_CENTER;

        Vec2 offset {0.f};
        Vec2 size   {100.f, 100.f};
        Vec2 pivot  {0.f}; // Pivot is strictly 0 - 1 
    };

    constexpr Vec2 GetAnchorScale(const Transform &_transform)
    {
        switch (_transform.anchor)
        {
        case Anchor::TOP_LEFT:
            return {0.f, 0.f};
        case Anchor::TOP_CENTER:
            return {0.5f, 0.f};
        case Anchor::TOP_RIGHT:
            return {1.f, 0.f};
        case Anchor::CENTER_LEFT:
            return {0.f, 0.5f};
        case Anchor::CENTER_CENTER:
            return {0.5f, 0.5f};
        case Anchor::CENTER_RIGHT:
            return {1.f, 0.5f};
        case Anchor::BOTTOM_LEFT:
            return {0.f, 1.f};
        case Anchor::BOTTOM_CENTER:
            return {0.5f, 1.f};
        case Anchor::BOTTOM_RIGHT:
            return {1.f, 1.f};
        default:
            return {0.f};
        }
    }

    constexpr Rect ResolvePosition(const Rect &_parent, const Transform &_child)
    {
        // Where N = child, C = parent, A = anchor, Cw/Ch = parent Width/Height, sx/sy = size x/y, px/py = pivot x/y, O = offsets
        // X: Nx = Cx + (Cw * Ax) + Ox - (Nsx * Npx)
        // Y: Ny = Cy + (Ch * Ay) + Oy - (Nsy * Npy)

        float nx, ny;
        Vec2 A = GetAnchorScale(_child);

        nx = _parent.position.x + (_parent.size.x * A.x) + _child.offset.x - (_child.size.x * _child.pivot.x);
        ny = _parent.position.y + (_parent.size.y * A.y) + _child.offset.y - (_child.size.y * _child.pivot.y);

        return Rect({nx, ny}, _child.size);
    }
} 
