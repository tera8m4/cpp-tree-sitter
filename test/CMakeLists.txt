cmake_minimum_required(VERSION 3.19)

project(cpp-tree-sitter-test LANGUAGES CXX)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:catchorg/Catch2@3.7.1")

add_grammar_from_repo(tree-sitter-cpp
  https://github.com/tree-sitter/tree-sitter-cpp
  0.23.4
)

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} cpp-tree-sitter tree-sitter-cpp Catch2::Catch2WithMain)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

enable_testing()

include(CTest)

include("${Catch2_SOURCE_DIR}/extras/Catch.cmake")

catch_discover_tests(${PROJECT_NAME})
