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

## CMAKE_BUILD_TYPE setup macro ##

macro(tsimd_setup_build_type)
  # force 'Release' build type on initial configuration (otherwise is empty)
  set(CONFIGURATION_TYPES "Debug" "Release" "RelWithDebInfo")
  if (WIN32)
    if (NOT DEFAULT_CMAKE_CONFIGURATION_TYPES_SET)
      set(CMAKE_CONFIGURATION_TYPES "${CONFIGURATION_TYPES}"
          CACHE STRING "List of generated configurations." FORCE)
      set(DEFAULT_CMAKE_CONFIGURATION_TYPES_SET ON CACHE INTERNAL
          "Default CMake configuration types set.")
    endif()
  else()
    if(NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE "Release" CACHE STRING
          "Choose the type of build." FORCE)
      set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
                   STRINGS ${CONFIGURATION_TYPES})
    endif()
  endif()

  # we are only building tests, should probably stick with 'Release'
  mark_as_advanced(CMAKE_BUILD_TYPE)
endmacro()

## Compiler configuration macro ##

macro(tsimd_configure_compiler)

  tsimd_configure_isa()

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    include(icc)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    include(gcc)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    include(clang)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    include(clang)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    include(msvc)
  else()
    message(FATAL_ERROR "Unsupported compiler: '${CMAKE_CXX_COMPILER_ID}'")
  endif()

  mark_as_advanced(CLEAR CMAKE_CXX_COMPILER)

  find_package(OpenMP)
  if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  endif()
endmacro()

include(clang-format)
