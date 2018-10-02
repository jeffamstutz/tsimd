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

##############################################################
# CMake configuration
##############################################################

cmake_minimum_required(VERSION 3.2)

project(tsimd)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(tsimd)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

##############################################################
# Main tsimd library CMake target
##############################################################

add_library(tsimd INTERFACE)
target_include_directories(tsimd INTERFACE
  $<INSTALL_INTERFACE:include/tsimd>
)

install(DIRECTORY tsimd DESTINATION include)
install(TARGETS tsimd
	EXPORT tsimdConfig
	RUNTIME DESTINATION bin/tsimd
	ARCHIVE DESTINATION lib/tsimd
  LIBRARY DESTINATION lib/tsimd
)
install(EXPORT tsimdConfig DESTINATION lib/cmake/tsimd)

##############################################################
# Build tests, examples, and benchmarks
##############################################################

include(ispc)

tsimd_setup_build_type()
tsimd_configure_compiler()

if ((TSIMD_BUILD_ISA STREQUAL "NATIVE" OR TSIMD_BUILD_ISA STREQUAL "SCALAR")
     AND TSIMD_ENABLE_ISPC_COMPARISONS)
  message(FATAL_ERROR
          "You cannot use the 'NATIVE' or 'SCALAR' ISA target with ispc.")
endif()

include_directories(${CMAKE_SOURCE_DIR})

option(TSIMD_BUILD_TESTS "Build unit tests" OFF)
option(TSIMD_BUILD_EXAMPLES "Build examples" ON)
option(TSIMD_BUILD_BENCHMARKS "Build benchmarks" OFF)

## Unit tests ##

if (TSIMD_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

## Extra examples ##

if (TSIMD_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

## Benchmarks ##

if (TSIMD_BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()