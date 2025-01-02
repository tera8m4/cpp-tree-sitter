#include <language.h>
#include <node.h>
#include <parser.h>
#include <tree.h>

#include <cassert>
#include <cstdio>
#include <memory>
#include <string>

extern "C" {
TSLanguage* tree_sitter_cpp();
}

int main() {
  // Create a language and parser.
  ts::Language language = tree_sitter_cpp();
  ts::Parser parser{language};

  // Parse the provided string into a syntax tree.
  std::string sourcecode =
      "int main() { int i = 42; std::cout << i; return 0; }";
  ts::Tree tree = parser.parseString(sourcecode);

  // Get the root node of the syntax tree.
  ts::Node root = tree.getRootNode();

  // Get some child nodes.
  ts::Node array = root.getNamedChild(0);
  ts::Node number = array.getNamedChild(0);

  // Print the syntax tree as an S-expression.
  auto treestring = root.getSExpr();
  printf("Syntax tree: %s\n", treestring.get());

  return 0;
}
