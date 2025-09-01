#include "cpp-tree-sitter/cursor.h"
#include "cpp-tree-sitter/node.h"

namespace ts {
[[nodiscard]] Cursor Node::getCursor() const { return Cursor{impl}; }
}  // namespace ts
