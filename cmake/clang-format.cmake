# additional target to run clang-format on all source files

set (EXCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/build
  ${CMAKE_SOURCE_DIR}/examples/embc
  ${CMAKE_SOURCE_DIR}/tests/doctest
)

# get all project files
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h *.ih *.ispc)
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
  foreach (EXCLUDE_DIR ${EXCLUDE_DIRS})
    string(FIND ${SOURCE_FILE} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
      list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
    endif()
  endforeach()
endforeach()

add_custom_target(
  format
  COMMAND clang-format
  -style=file
  -i
  ${ALL_SOURCE_FILES}
)
