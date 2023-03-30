cmake_minimum_required(VERSION 3.0)

project(moolisp VERSION 0.1
  DESCRIPTION "A lisp"
  LANGUAGES C)
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
)

enable_testing()
add_subdirectory(src)
add_subdirectory(modules)
add_subdirectory(test EXCLUDE_FROM_ALL)
