cmake_minimum_required(VERSION 3.0)

project(moolisp VERSION 0.1
  DESCRIPTION "A lisp"
  LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

enable_testing()
add_subdirectory(src)
add_subdirectory(test EXCLUDE_FROM_ALL)
