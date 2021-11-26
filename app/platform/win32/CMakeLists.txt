include(sources.cmake)
add_executable(puzzle15 WIN32 ${sources})

target_include_directories(puzzle15 PRIVATE src)
target_compile_definitions(puzzle15 PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN UNICODE _UNICODE)
target_link_libraries(puzzle15 PRIVATE puzzle15::lib Comctl32.lib)
target_precompile_headers(puzzle15 PRIVATE src/puzzle15/platform/stdafx.h)