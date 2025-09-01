#pragma once

#include <tree_sitter/api.h>

#include <memory>
#include <string_view>

#include "aliases.h"
#include "language.h"

namespace ts {
struct FreeHelper {
  template <typename T>
  void operator()(T* raw_pointer) const {
    std::free(raw_pointer);
  }
};

// An inclusive range representation
template <typename T>
struct Extent {
  T start;
  T end;
};

/////////////////////////////////////////////////////////////////////////////
// Aliases.
// Create slightly stricter aliases for some of the core tree-sitter types.
/////////////////////////////////////////////////////////////////////////////
class Cursor;
struct Node {
  explicit Node(TSNode node) : impl{node} {}

  ////////////////////////////////////////////////////////////////
  // Flag checks on nodes
  ////////////////////////////////////////////////////////////////
  [[nodiscard]] bool isNull() const { return ts_node_is_null(impl); }

  [[nodiscard]] bool isNamed() const { return ts_node_is_named(impl); }

  [[nodiscard]] bool isMissing() const { return ts_node_is_missing(impl); }

  [[nodiscard]] bool isExtra() const { return ts_node_is_extra(impl); }

  [[nodiscard]] bool hasError() const { return ts_node_has_error(impl); }

  [[nodiscard]] bool isError() const { return ts_node_is_error(impl); }

  ////////////////////////////////////////////////////////////////
  // Navigation
  ////////////////////////////////////////////////////////////////

  // Direct parent/sibling/child connections and cursors

  [[nodiscard]] Node getParent() const { return Node{ts_node_parent(impl)}; }

  [[nodiscard]] Node getPreviousSibling() const {
    return Node{ts_node_prev_sibling(impl)};
  }

  [[nodiscard]] Node getNextSibling() const {
    return Node{ts_node_next_sibling(impl)};
  }

  [[nodiscard]] uint32_t getNumChildren() const {
    return ts_node_child_count(impl);
  }

  [[nodiscard]] Node getChild(uint32_t position) const {
    return Node{ts_node_child(impl, position)};
  }

  // Named children

  [[nodiscard]] uint32_t getNumNamedChildren() const {
    return ts_node_named_child_count(impl);
  }

  [[nodiscard]] Node getNamedChild(uint32_t position) const {
    return Node{ts_node_named_child(impl, position)};
  }

  // Named fields

  [[nodiscard]] std::string_view getFieldNameForChild(
      uint32_t child_position) const {
    return ts_node_field_name_for_child(impl, child_position);
  }

  [[nodiscard]] Node getChildByFieldName(std::string_view name) const {
    return Node{ts_node_child_by_field_name(
        impl, &name.front(), static_cast<uint32_t>(name.size()))};
  }

  // Definition deferred until after the definition of Cursor.
  [[nodiscard]] Cursor getCursor() const;
  ////////////////////////////////////////////////////////////////
  // Node attributes
  ////////////////////////////////////////////////////////////////

  // Returns a unique identifier for a node in a parse tree.
  // NodeIDs are numeric value types.
  [[nodiscard]] NodeID getID() const {
    return reinterpret_cast<NodeID>(impl.id);
  }

  // Returns an S-Expression representation of the subtree rooted at this node.
  [[nodiscard]] std::unique_ptr<char, FreeHelper> getSExpr() const {
    return std::unique_ptr<char, FreeHelper>{ts_node_string(impl)};
  }

  [[nodiscard]] Symbol getSymbol() const { return ts_node_symbol(impl); }

  [[nodiscard]] std::string_view getType() const { return ts_node_type(impl); }

  [[nodiscard]] Language getLanguage() const { return ts_node_language(impl); }

  [[nodiscard]] Extent<uint32_t> getByteRange() const {
    return {ts_node_start_byte(impl), ts_node_end_byte(impl)};
  }

  [[nodiscard]] Extent<Point> getPointRange() const {
    return {ts_node_start_point(impl), ts_node_end_point(impl)};
  }

  [[nodiscard]] std::string_view getSourceRange(std::string_view source) const {
    Extent<uint32_t> extents = this->getByteRange();
    return source.substr(extents.start, extents.end - extents.start);
  }

  TSNode impl;
};
}  // namespace ts
