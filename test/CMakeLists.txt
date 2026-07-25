add_executable(test_container test_container.cpp)
target_link_libraries(test_container PRIVATE phase_lib doctest::doctest)
add_test(NAME test_container COMMAND test_container)
