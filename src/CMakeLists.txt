add_custom_command(
  OUTPUT lexer.c lexer.h
  COMMAND flex --header-file="${CMAKE_BINARY_DIR}/src/lexer.h" -f -o "${CMAKE_BINARY_DIR}/src/lexer.c" lexer.lex
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src"
  DEPENDS lexer.lex
)

add_library(lexer lexer.c)
target_compile_options(lexer PRIVATE -Wno-unused-function)

add_library(tgc tgc/tgc.c)
target_compile_options(tgc PRIVATE -Wno-use-after-free)

add_library(structures hashmap.c linked_list.c)
target_link_libraries(structures PRIVATE tgc)

add_executable("${PROJECT_NAME}" lisp.c)
target_link_libraries("${PROJECT_NAME}" PRIVATE lexer tgc)
