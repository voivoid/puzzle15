include(FetchContent)

FetchContent_Declare(
  get_catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_TAG        v3.0.0-preview3
)

FetchContent_GetProperties(get_catch2)
if(NOT get_catch2_POPULATED)
  FetchContent_Populate(get_catch2)

  set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1) 
  add_subdirectory(${get_catch2_SOURCE_DIR} ${get_catch2_BINARY_DIR})
endif()

