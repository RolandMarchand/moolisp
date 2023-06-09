cmake_minimum_required(VERSION 3.0)

project(moolisp VERSION 0.2.0
  DESCRIPTION "A lisp"
  LANGUAGES C)
set(PROJECT_NAME_PRETTY "Moolisp")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

include_directories(
  "${CMAKE_SOURCE_DIR}/modules"
  "${CMAKE_SOURCE_DIR}/src"
  "${PROJECT_BINARY_DIR}/src"
)

file(STRINGS "${CMAKE_SOURCE_DIR}/LICENSE" LICENSE)
string(REPLACE ";" "\\n" LICENSE "${LICENSE}")
string(REPLACE "\"" "\\\"" LICENSE "${LICENSE}")
string(APPEND LICENSE "\\n")

configure_file(
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_BINARY_DIR}/src/config.h"
)

enable_testing()
add_subdirectory(src)
add_subdirectory(modules)
add_subdirectory(test EXCLUDE_FROM_ALL)
