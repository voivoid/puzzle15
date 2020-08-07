add_executable(puzzle15_tests board.cpp)
target_link_libraries(puzzle15_tests PRIVATE puzzle15::lib Catch2::Catch2WithMain)

add_test(NAME puzzle15_tests
         COMMAND $<TARGET_FILE:puzzle15_tests>)