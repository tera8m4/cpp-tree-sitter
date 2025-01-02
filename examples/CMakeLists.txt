# Minimum required version of CMake
cmake_minimum_required(VERSION 3.14)

# Project name and version
project(cpp-tree-sitter-example VERSION 1.0 LANGUAGES CXX)

add_grammar_from_repo(tree-sitter-cpp
  https://github.com/tree-sitter/tree-sitter-cpp
  0.23.4
)

# Add your example source file
set(SOURCES main.cpp)

# Create the executable
add_executable(cpp-tree-sitter-example ${SOURCES})

# Link the cpp-tree-sitter library to your executable
target_link_libraries(cpp-tree-sitter-example tree-sitter-cpp cpp-tree-sitter)
