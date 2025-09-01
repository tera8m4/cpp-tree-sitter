#include <catch2/catch_test_macros.hpp>

#include "cpp-tree-sitter/language.h"
#include "cpp-tree-sitter/parser.h"
#include "cpp-tree-sitter/query.h"

using namespace ts;

extern "C" {
TSLanguage* tree_sitter_cpp();
}

TEST_CASE("Query constructor initializes correctly", "[Query]") {
  Language lang = tree_sitter_cpp();
  std::string_view query_source = "some_query_source";

  SECTION("Query object is created without errors") {
    REQUIRE_NOTHROW(Query(lang, query_source));
  }
}

TEST_CASE("Query hasError method works correctly", "[Query]") {
  Language lang = tree_sitter_cpp();
  std::string_view valid_query_source = "(function_definition) @context";
  std::string_view invalid_query_source = "invalid_query_source";

  SECTION("Query with valid source has no error") {
    Query valid_query(lang, valid_query_source);
    REQUIRE_FALSE(valid_query.hasError());
  }

  SECTION("Query with invalid source has error") {
    Query invalid_query(lang, invalid_query_source);
    REQUIRE(invalid_query.hasError());
  }
}

TEST_CASE("Query properties", "[Query]") {
  auto language = tree_sitter_cpp();
  std::string_view query_source =
      "(function_declarator declarator: (identifier) @function_name)";

  SECTION("Valid Query Property Access") {
    Query query(language, query_source);

    REQUIRE_FALSE(query.hasError());
    REQUIRE(query.getPatternCount() > 0);
    REQUIRE(query.getCaptureCount() > 0);
  }

  SECTION("Iterating over query") {
    Query q(language, query_source);
    ts::Parser parser{language};

    std::string_view sourcecode =
        "void foo() { std::cout << 123; } int main() { int i = 42; std::cout "
        "<< i; return 0; }";
    ts::Tree tree = parser.parseString(sourcecode);

    ts::Node root = tree.getRootNode();

    QueryCursor cursor = q.exec(root);
    auto begin = cursor.begin();
    auto end = cursor.end();
    REQUIRE(begin != end);
    int k = 0;

    std::string_view functions[2] = {"foo", "main"};

    for (auto it = begin; it != end; ++it) {
      REQUIRE(it->getCaptureCount() > 0);
      const auto& captures = it->getCaptures();
      REQUIRE(captures.size() == 1);
      const auto range = captures[0].getNode().getByteRange();

      const std::string_view nodeText =
          sourcecode.substr(range.start, range.end - range.start);
      REQUIRE(nodeText == functions[k]);

      ++k;
    }

    REQUIRE(k == 2);
  }
}
