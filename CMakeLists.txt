cmake_minimum_required(VERSION 3.19)
project(cpp-tree-sitter)

set(PACKAGE_NAME cpp-tree-sitter)
set(PACKAGE_VERSION 0.0.3)
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

add_compile_options(
  "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall;-Wextra;-Wconversion>"
)

include(cmake/CPM.cmake)

set(CPP_TREESITTER_ENABLE_EXAMPLES OFF CACHE BOOL
  "Enable example")
set(CPP_TREESITTER_ENABLE_TEST OFF CACHE BOOL
  "Enable tests")

# We want to automatically download and provide tree-sitter to users of
# the package, so pull it in and retrofit cmake dependencies on top of it.
CPMAddPackage(
  NAME tree-sitter
  GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter.git
  VERSION 0.22.6
  DOWNLOAD_ONLY YES
)

if (tree-sitter_ADDED)
  add_library(tree-sitter)
  target_sources(tree-sitter
    PRIVATE
      "${tree-sitter_SOURCE_DIR}/lib/src/lib.c"
  )
  target_include_directories(tree-sitter
    PRIVATE
      $<BUILD_INTERFACE:${tree-sitter_SOURCE_DIR}/lib/src>
    PUBLIC
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${tree-sitter_SOURCE_DIR}/lib/include>
  )
  target_compile_options(tree-sitter
    PRIVATE
      "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-conversion>"
  )

  # Install tree-sitter
  install(TARGETS tree-sitter
          EXPORT ${PROJECT_NAME}Targets
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib)
  install(DIRECTORY ${tree-sitter_SOURCE_DIR}/lib/include/
          DESTINATION include)
endif()

function(add_grammar_from_repo NAME REPO VERSION)
  CPMAddPackage(
    NAME ${NAME}
    GIT_REPOSITORY ${REPO}
    VERSION ${VERSION}
    DOWNLOAD_ONLY YES
  )

  if ("${${NAME}_ADDED}")
    add_library(${NAME})

    file(GLOB maybe_scanner "${${NAME}_SOURCE_DIR}/src/scanner.c")
    target_sources(${NAME}
      PRIVATE
        "${${NAME}_SOURCE_DIR}/src/parser.c"
        ${maybe_scanner}
    )
    target_include_directories(${NAME}
      PRIVATE
        $<BUILD_INTERFACE:${${NAME}_SOURCE_DIR}/src>
      PUBLIC
        $<INSTALL_INTERFACE:include>
    )

    target_link_libraries(${NAME}
      INTERFACE
        tree-sitter
    )
    target_compile_options(${NAME}
      PRIVATE
        "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-unused-but-set-variable>"
    )

    # Install grammar library (only if not already installed)
    if (NOT TARGET ${NAME}-installed)
      install(TARGETS ${NAME}
              EXPORT ${PROJECT_NAME}Targets
              RUNTIME DESTINATION bin
              LIBRARY DESTINATION lib
              ARCHIVE DESTINATION lib)
      set(${NAME}-installed TRUE CACHE INTERNAL "")
    endif()
  endif()
endfunction(add_grammar_from_repo)

# ---- Add source files ----

file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# ---- Create library ----

add_library(${PROJECT_NAME} ${headers} ${sources})

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

target_link_libraries(cpp-tree-sitter PRIVATE tree-sitter)

# Install cpp-tree-sitter
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION})

# Export targets
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION lib/cmake/${PROJECT_NAME})

if (CPP_TREESITTER_ENABLE_EXAMPLES)
  add_subdirectory(examples)
endif()
if (CPP_TREESITTER_ENABLE_TEST)
  add_subdirectory(test)
endif()
