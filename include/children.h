#pragma once

#include <iterator>

#include "cursor.h"
#include "node.h"

namespace ts {
////////////////////////////////////////////////////////////////
// Child node iterators
////////////////////////////////////////////////////////////////

// These iterators make it possible to use C++ views on Nodes for
// easy processing.

class ChildIteratorSentinel {};

class ChildIterator {
 public:
  using value_type = ts::Node;
  using difference_type = int;
  using iterator_category = std::input_iterator_tag;

  explicit ChildIterator(const ts::Node& node)
      : cursor{node.getCursor()}, atEnd{!cursor.gotoFirstChild()} {}

  value_type operator*() const { return cursor.getCurrentNode(); }

  ChildIterator& operator++() {
    atEnd = !cursor.gotoNextSibling();
    return *this;
  }

  ChildIterator& operator++(int) {
    atEnd = !cursor.gotoNextSibling();
    return *this;
  }

  friend bool operator==(const ChildIterator& a, const ChildIteratorSentinel&) {
    return a.atEnd;
  }
  friend bool operator!=(const ChildIterator& a,
                         const ChildIteratorSentinel& b) {
    return !(a == b);
  }
  friend bool operator==(const ChildIteratorSentinel& b,
                         const ChildIterator& a) {
    return a == b;
  }
  friend bool operator!=(const ChildIteratorSentinel& b,
                         const ChildIterator& a) {
    return a != b;
  }

 private:
  ts::Cursor cursor;
  bool atEnd;
};

struct Children {
  using iterator = ChildIterator;
  using sentinel = ChildIteratorSentinel;

  auto begin() const -> iterator { return ChildIterator{node}; }
  auto end() const -> sentinel { return {}; }
  const ts::Node& node;
};

static_assert(std::input_iterator<ChildIterator>);
static_assert(std::sentinel_for<ChildIteratorSentinel, ChildIterator>);
}  // namespace ts
