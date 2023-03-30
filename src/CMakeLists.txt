add_library(lexer lexer.c)
target_link_libraries(lexer PUBLIC tgc)

add_library(structures hashmap.c linked_list.c var.c)
target_link_libraries(structures PUBLIC tgc)

find_package(Readline REQUIRED)

add_library(repl repl.c)
target_link_libraries(repl PRIVATE ${Readline_LIBRARY})

add_executable("${PROJECT_NAME}" main.c)
target_link_libraries("${PROJECT_NAME}" PRIVATE lexer tgc structures repl)
