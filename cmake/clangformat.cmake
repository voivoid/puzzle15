find_program(ClangFormat clang-format)
if(ClangFormat)
  set(FormatTargets puzzle15 puzzle15_lib puzzle15_tests)
  foreach(FormatTarget ${FormatTargets})
    get_target_property(FormatTargetSources ${FormatTarget} SOURCES)
    get_target_property(FormatTargetDir ${FormatTarget} SOURCE_DIR)
    foreach(Src ${FormatTargetSources})
      list(APPEND FormatSources "${FormatTargetDir}/${Src}")
    endforeach()
  endforeach()

  list(FILTER FormatSources INCLUDE REGEX "^.*\.(h|cpp)\$")

  add_custom_target(clangformat
    COMMAND clang-format -style=file -i ${FormatSources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
