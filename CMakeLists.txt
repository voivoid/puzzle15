cmake_minimum_required(VERSION 3.22)

project(Puzzle15 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/MP /W4 /WX /permissive- /Zc:preprocessor)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

include(cmake/catch2.cmake)
include(cmake/boost.cmake)

add_subdirectory(app)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT puzzle15)

enable_testing()
add_subdirectory(tests)


include(cmake/clangformat.cmake)
