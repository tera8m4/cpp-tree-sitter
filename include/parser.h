#pragma once
#include <tree_sitter/api.h>

#include <memory>
#include <string_view>

#include "language.h"
#include "tree.h"

namespace ts {

class Parser {
 public:
  Parser(const Language& language) : impl{ts_parser_new(), ts_parser_delete} {
    ts_parser_set_language(impl.get(), language.impl);
  }

  [[nodiscard]] Tree parseString(std::string_view buffer) {
    return ts_parser_parse_string(impl.get(), nullptr, &buffer.front(),
                                  static_cast<uint32_t>(buffer.size()));
  }

 private:
  std::unique_ptr<TSParser, decltype(&ts_parser_delete)> impl;
};

}  // namespace ts
