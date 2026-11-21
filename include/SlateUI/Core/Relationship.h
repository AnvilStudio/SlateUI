#pragma once
#include "ElementHandle.h"

namespace slate {
struct Relationship {
    ElementHandle parent = null_handle;
    ElementHandle first_child = null_handle;
    ElementHandle next_sibling = null_handle;
    ElementHandle last_child = null_handle;
};
}