#pragma once

#include <cstdint>

namespace slate
{
    struct ElementHandle
    {
        uint32_t index = -1;
        uint32_t generation = -1;
    };
}