include_directories("${CMAKE_SOURCE_DIR}/src")
add_custom_target(tests DEPENDS test_lexer)

add_executable(test_lexer test_lexer.c)
target_link_libraries(test_lexer PUBLIC lexer)

add_test(Lexer test_lexer)
