#include "cpp-tree-sitter/language.h"
#include "cpp-tree-sitter/node.h"
#include "cpp-tree-sitter/query.h"

namespace ts {

QueryCursor::QueryCursor(const TSQuery* query, const Node& node)
    : impl{ts_query_cursor_new(), ts_query_cursor_delete} {
  ts_query_cursor_exec(impl.get(), query, node.impl);
}

Query::Query(const Language& language, std::string_view query_source) {
  uint32_t error_offset = 0;
  TSQueryError error_type;
  TSQuery* query =
      ts_query_new(language.impl, query_source.data(), query_source.size(),
                   &error_offset, &error_type);

  if (error_type == TSQueryErrorNone) {
    impl.reset(query);
  } else {
    ts_query_delete(query);
  }
}

uint32_t Query::getPatternCount() const {
  return ts_query_pattern_count(impl.get());
}

uint32_t Query::getCaptureCount() const {
  return ts_query_capture_count(impl.get());
}

uint32_t Query::getStringCount() const {
  return ts_query_string_count(impl.get());
}

bool Query::hasError() const { return impl == nullptr; }

QueryCursor Query::exec(const Node& node) const {
  return QueryCursor{impl.get(), node};
}

void QueryCursor::Iterator::advance() {
  bAtEnd = !ts_query_cursor_next_match(cursor, &match.impl);
}

}  // namespace ts
