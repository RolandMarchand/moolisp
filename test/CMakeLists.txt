include_directories("${CMAKE_SOURCE_DIR}/src")
add_custom_target(tests DEPENDS test_lexer test_hashmap test_linked_list)

add_executable(test_lexer test_lexer.c)
target_link_libraries(test_lexer PRIVATE lexer)

add_executable(test_hashmap test_hashmap.c)
target_link_libraries(test_hashmap PRIVATE structures tgc)

add_executable(test_linked_list test_linked_list.c)
target_link_libraries(test_linked_list PUBLIC structures tgc)

add_test(Lexer test_lexer)
add_test(Hashmap test_hashmap)
add_test(LinkedList test_linked_list)
