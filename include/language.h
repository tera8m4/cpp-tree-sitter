#pragma once

#include <tree_sitter/api.h>

#include <string_view>

#include "aliases.h"

namespace ts {
// For types that manage resources, create custom wrappers that ensure
// clean-up. For types that can benefit from additional API discovery,
// wrappers with implicit conversion allow for automated method discovery.

struct Language {
  // NOTE: Allowing implicit conversions from TSLanguage to Language
  // improves the ergonomics a bit, as clients will still make use of the
  // custom language functions.

  /* implicit */ Language(TSLanguage const* language) : impl{language} {}

  [[nodiscard]] size_t getNumSymbols() const {
    return ts_language_symbol_count(impl);
  }

  [[nodiscard]] std::string_view getSymbolName(Symbol symbol) const {
    return ts_language_symbol_name(impl, symbol);
  }

  [[nodiscard]] Symbol getSymbolForName(std::string_view name,
                                        bool isNamed) const {
    return ts_language_symbol_for_name(
        impl, &name.front(), static_cast<uint32_t>(name.size()), isNamed);
  }

  [[nodiscard]] Version getVersion() const { return ts_language_version(impl); }

  TSLanguage const* impl;
};
}  // namespace ts
