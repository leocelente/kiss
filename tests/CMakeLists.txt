add_executable(test-kiss test-kiss.c)
target_link_libraries(test-kiss kiss)
add_test(NAME test-kiss COMMAND test-kiss)
