#pragma once

#include <tree_sitter/api.h>

namespace ts {
// Direct alias of { row: uint32_t; column: uint32_t }
using Point = TSPoint;

using Symbol = uint16_t;

using Version = uint32_t;

using NodeID = uintptr_t;
}  // namespace ts
