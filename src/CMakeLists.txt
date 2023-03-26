add_custom_command(
  OUTPUT lexer.c lexer.h
  COMMAND flex --header-file="${CMAKE_BINARY_DIR}/src/lexer.h" -f -o "${CMAKE_BINARY_DIR}/src/lexer.c" lexer.lex
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src"
  DEPENDS lexer.lex
)

add_library(lexer lexer.c)
target_compile_options(lexer PUBLIC -Wno-unused-function)

add_library(tgc tgc/tgc.c)
target_compile_options(lexer PUBLIC -Wno-free-nonheap-object)

add_executable("${PROJECT_NAME}" lisp.c)
target_link_libraries("${PROJECT_NAME}" PUBLIC lexer tgc)

