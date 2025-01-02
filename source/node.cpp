#include "cursor.h"
#include "node.h"

namespace ts {
[[nodiscard]] Cursor Node::getCursor() const { return Cursor{impl}; }
}  // namespace ts
