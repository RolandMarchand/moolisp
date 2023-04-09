add_library(lexer lexer.c)
target_link_libraries(lexer PUBLIC tgc)

add_library(compiler parser.c)
target_link_libraries(compiler PRIVATE lexer PUBLIC structures)
target_include_directories(compiler PRIVATE "${CMAKE_SOURCE_DIR}/src")

add_library(structures hashmap.c var.c env.c)
target_link_libraries(structures PUBLIC tgc)

add_library(context context.c)
target_link_libraries(context PUBLIC tgc)

find_package(Readline REQUIRED)

add_library(repl repl.c eval.c core.c)
target_link_libraries(repl PRIVATE ${Readline_LIBRARY} compiler)

add_executable("${PROJECT_NAME}" main.c)
target_link_libraries("${PROJECT_NAME}" PRIVATE
  structures
  compiler
  context
  repl
  tgc
)
