#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>

namespace slate {
struct ElementHandle {
    size_t index = std::numeric_limits<size_t>::max();
    uint32_t generation = 0;

    constexpr bool operator==(const ElementHandle&) const = default;
};
inline constexpr ElementHandle null_handle{};
}