include(sources.cmake)

add_library(puzzle15_lib ${puzzle15_lib_sources})
add_library(puzzle15::lib ALIAS puzzle15_lib)

target_include_directories(puzzle15_lib PUBLIC inc PRIVATE src)
target_link_libraries(puzzle15_lib PRIVATE Boost::boost)

if(WIN32)
    add_subdirectory(platform/win32)
else()
    message(FATAL_ERROR "unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()