include_directories("${CMAKE_SOURCE_DIR}/src")
add_custom_target(tests DEPENDS
#  test_var
  test_lexer
#   test_compiler
  test_hashmap
  test_context
)
add_custom_target(test_and_run COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS tests)

add_executable(test_var test_var.c)
target_link_libraries(test_var PRIVATE structures)

add_executable(test_lexer test_lexer.c)
target_link_libraries(test_lexer PRIVATE lexer)

# add_executable(test_compiler test_compiler.c)
# target_link_libraries(test_compiler PRIVATE compiler)

add_executable(test_hashmap test_hashmap.c)
target_link_libraries(test_hashmap PRIVATE structures)

add_executable(test_context test_context.c)
target_link_libraries(test_context PRIVATE context)

#add_test(Var test_var)
add_test(Lexer test_lexer)
# add_test(Compiler test_compiler)
add_test(Hashmap test_hashmap)
add_test(Context test_context)
