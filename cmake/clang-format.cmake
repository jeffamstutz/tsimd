## ========================================================================== ##
## The MIT License (MIT)                                                      ##
##                                                                            ##
## Copyright (c) 2017 Intel Corporation                                       ##
##                                                                            ##
## Permission is hereby granted, free of charge, to any person obtaining a    ##
## copy of this software and associated documentation files (the "Software"), ##
## to deal in the Software without restriction, including without limitation  ##
## the rights to use, copy, modify, merge, publish, distribute, sublicense,   ##
## and/or sell copies of the Software, and to permit persons to whom the      ##
## Software is furnished to do so, subject to the following conditions:       ##
##                                                                            ##
## The above copyright notice and this permission notice shall be included in ##
## in all copies or substantial portions of the Software.                     ##
##                                                                            ##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR ##
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   ##
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    ##
## THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER ##
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    ##
## FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        ##
## DEALINGS IN THE SOFTWARE.                                                  ##
## ========================================================================== ##

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
