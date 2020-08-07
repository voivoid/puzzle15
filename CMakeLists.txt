cmake_minimum_required(VERSION 3.18)

project(Puzzle15 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/MP /W4 /WX /permissive-)
else()
  add_compile_options(puzzle15_lib PUBLIC -Wall -Wextra -Wpedantic -Werror)
endif()

include(cmake/catch2.cmake)

add_subdirectory(app)

enable_testing()
add_subdirectory(tests)


include(cmake/clangformat.cmake)