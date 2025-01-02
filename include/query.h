#pragma once

#include <tree_sitter/api.h>

#include <iterator>
#include <memory>
#include <span>
#include <string_view>

#include "aliases.h"

namespace ts {

struct Language;
struct Node;

struct QueryCapture : protected TSQueryCapture {
  Node getNode() const { return Node{node}; }
  uint32_t getIndex() const { return index; }
};

struct QueryMatch {
  using ID = uint32_t;
  using Capture = QueryCapture;

  inline ID getId() const { return impl.id; }
  inline uint32_t getPatternIndex() const { return impl.pattern_index; }
  inline uint16_t getCaptureCount() const { return impl.capture_count; }

  std::span<const Capture> getCaptures() const {
    return std::span(reinterpret_cast<const Capture*>(impl.captures),
                     impl.capture_count);
  }

  TSQueryMatch impl;
};

class QueryCursor {
 public:
  explicit QueryCursor(const TSQuery* query, const Node& node);

  class Iterator {
   public:
    using value_type = QueryMatch;
    using difference_type = int;
    using iterator_category = std::input_iterator_tag;

    explicit Iterator(TSQueryCursor* inCursor, bool bInAtEnd)
        : cursor{inCursor}, bAtEnd{bInAtEnd} {
      if (!bAtEnd) {
        advance();
      }
    }

    value_type operator*() const { return match; }
    const value_type* operator->() const { return &match; }

    inline Iterator& operator++() {
      advance();
      return *this;
    }

    inline Iterator& operator++(int) {
      advance();
      return *this;
    }

    inline bool operator!=(const Iterator& other) const {
      return bAtEnd != other.bAtEnd;
    }

   private:
    TSQueryCursor* cursor = nullptr;
    bool bAtEnd = false;
    QueryMatch match;

    void advance();
  };

  Iterator begin() { return Iterator{impl.get(), false}; }
  Iterator end() { return Iterator{impl.get(), true}; }

 private:
  std::unique_ptr<TSQueryCursor, decltype(&ts_query_cursor_delete)> impl;
};

static_assert(std::input_iterator<QueryCursor::Iterator>);

class Query {
 public:
  explicit Query(const Language& language, std::string_view query_source);
  bool hasError() const;

  uint32_t getPatternCount() const;
  uint32_t getCaptureCount() const;
  uint32_t getStringCount() const;

  QueryCursor exec(const Node& node) const;

 private:
  std::unique_ptr<TSQuery, decltype(&ts_query_delete)> impl{nullptr,
                                                            ts_query_delete};
};
}  // namespace ts
