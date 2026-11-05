#pragma once

#include <cstdint>

namespace slate
{
    struct ElementHandle
    {
        size_t index = -1;
        uint32_t generation = -1;
    };
}