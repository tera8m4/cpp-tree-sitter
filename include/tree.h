#pragma once

#include <tree_sitter/api.h>

#include <memory>

#include "language.h"
#include "node.h"

namespace ts {

class Tree {
 public:
  Tree(TSTree* tree) : impl{tree, ts_tree_delete} {}

  [[nodiscard]] Node getRootNode() const {
    return Node{ts_tree_root_node(impl.get())};
  }

  [[nodiscard]] Language getLanguage() const {
    return Language{ts_tree_language(impl.get())};
  }

  [[nodiscard]] bool hasError() const { return getRootNode().hasError(); }

 private:
  std::unique_ptr<TSTree, decltype(&ts_tree_delete)> impl;
};
}  // namespace ts
