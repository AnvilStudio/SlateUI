#pragma once
#include "ElementHandle.h"

namespace slate
{
    struct Relationship
    {
        ElementHandle parent;
        ElementHandle first_child;
        ElementHandle next_sibling;
        ElementHandle last_child;
    };
}