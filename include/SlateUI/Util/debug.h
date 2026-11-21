#pragma once

#include <SlateUI/slate.h>
#include <iostream>
#include <string>

namespace slate::debug
{
    inline void PrintNode(
        ElementHandle node,
        const Context& ctx,
        const std::string& prefix,
        bool isLast)
    {
        if (!IsValid(node, ctx))
            return;

        const auto& slot = ctx.pool.slots[node.index];

        std::cout << prefix
                  << (isLast ? "└── " : "├── ")
                  << "Element [" << node.index << "]"
                  << " (gen: " << node.generation << ")\n";

        ElementHandle child = slot.relationship.first_child;

        const std::string childPrefix =
            prefix + (isLast ? "    " : "│   ");

        while (child != null_handle)
        {
            if (!IsValid(child, ctx))
            {
                std::cout << childPrefix
                          << "└── [INVALID HANDLE]\n";
                break;
            }

            const auto& childSlot = ctx.pool.slots[child.index];

            ElementHandle next =
                childSlot.relationship.next_sibling;

            PrintNode(
                child,
                ctx,
                childPrefix,
                next == null_handle
            );

            child = next;
        }
    }

    inline void PrintTree(ElementHandle root, const Context& ctx)
    {
        std::cout << "SlateUI Hierarchy\n";
        PrintNode(root, ctx, "", true);
    }
}