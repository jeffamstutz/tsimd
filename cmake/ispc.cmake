## ======================================================================== ##
## Copyright 2009-2017 Intel Corporation                                    ##
##                                                                          ##
## Licensed under the Apache License, Version 2.0 (the "License");          ##
## you may not use this file except in compliance with the License.         ##
## You may obtain a copy of the License at                                  ##
##                                                                          ##
##     http://www.apache.org/licenses/LICENSE-2.0                           ##
##                                                                          ##
## Unless required by applicable law or agreed to in writing, software      ##
## distributed under the License is distributed on an "AS IS" BASIS,        ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. ##
## See the License for the specific language governing permissions and      ##
## limitations under the License.                                           ##
## ======================================================================== ##

# ISPC versions to look for, in decending order (newest first)
set(ISPC_VERSION_WORKING "1.9.2" "1.9.1")
list(GET ISPC_VERSION_WORKING -1 ISPC_VERSION_REQUIRED)

IF (NOT ISPC_EXECUTABLE)
  # Try sibling folder as hint for path of ISPC
  if (APPLE)
    set(ISPC_DIR_SUFFIX "osx")
  elseif(WIN32)
    set(ISPC_DIR_SUFFIX "windows")
    if (MSVC_VERSION LESS 1900)
      list(APPEND ISPC_DIR_SUFFIX "windows-vs2013")
    else()
      list(APPEND ISPC_DIR_SUFFIX "windows-vs2015")
    endif()
  else()
    set(ISPC_DIR_SUFFIX "linux")
  endif()
  foreach(ver ${ISPC_VERSION_WORKING})
    foreach(suffix ${ISPC_DIR_SUFFIX})
      list(APPEND ISPC_DIR_HINT ${PROJECT_SOURCE_DIR}/../ispc-v${ver}-${suffix})
    endforeach()
  endforeach()

  find_program(ISPC_EXECUTABLE ispc HINTS ${ISPC_DIR_HINT} DOC "Path to the ISPC executable.")
  if (NOT ISPC_EXECUTABLE)
    message(STATUS "Could not find ISPC...disabling ISPC examples")
    set(TSIMD_ENABLE_ISPC OFF)
  else()
    message(STATUS "Found Intel SPMD Compiler (ISPC): ${ISPC_EXECUTABLE}")
  endif()
endif()

if(TSIMD_ENABLE_ISPC)
  if(NOT ISPC_VERSION)
    execute_process(COMMAND ${ISPC_EXECUTABLE} --version OUTPUT_VARIABLE ISPC_OUTPUT)
    string(REGEX MATCH " ([0-9]+[.][0-9]+[.][0-9]+)(dev|knl)? " DUMMY "${ISPC_OUTPUT}")
    set(ISPC_VERSION ${CMAKE_MATCH_1})

    if (ISPC_VERSION VERSION_LESS ISPC_VERSION_REQUIRED)
      message(FATAL_ERROR "Need at least version ${ISPC_VERSION_REQUIRED} of Intel SPMD Compiler (ISPC).")
    endif()

    set(ISPC_VERSION ${ISPC_VERSION} CACHE STRING "ISPC Version")
    mark_as_advanced(ISPC_VERSION)
    mark_as_advanced(ISPC_EXECUTABLE)
  endif()

  get_filename_component(ISPC_DIR ${ISPC_EXECUTABLE} PATH)
endif()

# ##################################################################
# add macro INCLUDE_DIRECTORIES_ISPC() that allows to specify search
# paths for ISPC sources
# ##################################################################
set(ISPC_INCLUDE_DIR "")
macro (INCLUDE_DIRECTORIES_ISPC)
  set(ISPC_INCLUDE_DIR ${ISPC_INCLUDE_DIR} ${ARGN})
endmacro ()

macro (TSIMD_ISPC_COMPILE)
  set(ISPC_ADDITIONAL_ARGS "")
  set(ISPC_TARGETS ${TSIMD_ISPC_TARGET_LIST})

  set(ISPC_TARGET_EXT ${CMAKE_CXX_OUTPUT_EXTENSION})
  string(REPLACE ";" "," ISPC_TARGET_ARGS "${ISPC_TARGETS}")

  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ISPC_ARCHITECTURE "x86-64")
  else()
    set(ISPC_ARCHITECTURE "x86")
  endif()

  set(ISPC_TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR})
  include_directories(${ISPC_TARGET_DIR})

  if(ISPC_INCLUDE_DIR)
    string(REPLACE ";" ";-I;" ISPC_INCLUDE_DIR_PARMS "${ISPC_INCLUDE_DIR}")
    set(ISPC_INCLUDE_DIR_PARMS "-I" ${ISPC_INCLUDE_DIR_PARMS})
  endif()

  if (WIN32 OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(ISPC_OPT_FLAGS -O3)
  else()
    set(ISPC_OPT_FLAGS -O2 -g)
  endif()

  if (NOT WIN32)
    set(ISPC_ADDITIONAL_ARGS ${ISPC_ADDITIONAL_ARGS} --pic)
  endif()

  if (NOT TSIMD_DEBUG_BUILD)
    set(ISPC_ADDITIONAL_ARGS ${ISPC_ADDITIONAL_ARGS} --opt=disable-assertions)
  endif()

  set(ISPC_OBJECTS "")

  foreach(src ${ARGN})
    get_filename_component(fname ${src} NAME_WE)
    get_filename_component(dir ${src} PATH)

    set(input ${src})
    if ("${dir}" MATCHES "^/") # absolute unix-style path to input
      set(outdir "${ISPC_TARGET_DIR}/rebased${dir}")
    elseif ("${dir}" MATCHES "^[A-Z]:") # absolute DOS-style path to input
      string(REGEX REPLACE "^[A-Z]:" "${ISPC_TARGET_DIR}/rebased/" outdir "${dir}")
    else() # relative path to input
      set(outdir "${ISPC_TARGET_DIR}/local_${dir}")
      set(input ${CMAKE_CURRENT_SOURCE_DIR}/${src})
    endif()

    set(deps "")
    if (EXISTS ${outdir}/${fname}.dev.idep)
      file(READ ${outdir}/${fname}.dev.idep contents)
      string(REPLACE " " ";"     contents "${contents}")
      string(REPLACE ";" "\\\\;" contents "${contents}")
      string(REPLACE "\n" ";"    contents "${contents}")
      foreach(dep ${contents})
        if (EXISTS ${dep})
          set(deps ${deps} ${dep})
        endif (EXISTS ${dep})
      endforeach(dep ${contents})
    endif ()

    set(results "${outdir}/${fname}.dev${ISPC_TARGET_EXT}")

    # if we have multiple targets add additional object files
    list(LENGTH ISPC_TARGETS NUM_TARGETS)
    if (NUM_TARGETS GREATER 1)
      foreach(target ${ISPC_TARGETS})
        string(REPLACE "-i32x16" "" target ${target}) # strip avx512(knl|skx)-i32x16
        set(results ${results} "${outdir}/${fname}.dev_${target}${ISPC_TARGET_EXT}")
      endforeach()
    endif()

    add_custom_command(
      OUTPUT ${results} ${ISPC_TARGET_DIR}/${fname}_ispc.h
      COMMAND ${CMAKE_COMMAND} -E make_directory ${outdir}
      COMMAND ${ISPC_EXECUTABLE}
      -I ${CMAKE_CURRENT_SOURCE_DIR}
      ${ISPC_INCLUDE_DIR_PARMS}
      --arch=${ISPC_ARCHITECTURE}
      --addressing=32
      ${ISPC_OPT_FLAGS}
      --target=${ISPC_TARGET_ARGS}
      --woff
      --opt=fast-math
      ${ISPC_ADDITIONAL_ARGS}
      -h ${ISPC_TARGET_DIR}/${fname}_ispc.h
      -MMM  ${outdir}/${fname}.dev.idep
      -o ${outdir}/${fname}.dev${ISPC_TARGET_EXT}
      ${input}
      DEPENDS ${input} ${deps}
      COMMENT "Building ISPC object ${outdir}/${fname}.dev${ISPC_TARGET_EXT}"
    )

    set(ISPC_OBJECTS ${ISPC_OBJECTS} ${results})
  endforeach()
endmacro()

## Macro configure ISA targets for ispc ##
macro(TSIMD_CONFIGURE_ISA)
  set(TSIMD_BENCHMARK_ISA "NATIVE" CACHE STRING
      "Target ISA for benchmarks (NATIVE, SCALAR, SSE4, AVX, AVX2, AVX512KNL, AVX512SKX)")
  string(TOUPPER ${TSIMD_BENCHMARK_ISA} TSIMD_BENCHMARK_ISA)
  set_property(CACHE TSIMD_BENCHMARK_ISA PROPERTY STRINGS
               NATIVE SCALAR SSE4 AVX AVX2 AVX512KNL AVX512SKX)

  unset(TSIMD_ISPC_TARGET_LIST)

  if (TSIMD_BENCHMARK_ISA STREQUAL "NATIVE")
    #TODO
  elseif (TSIMD_BENCHMARK_ISA STREQUAL "AVX512SKX")
    set(TSIMD_ISPC_TARGET_LIST avx512skx-i32x16)
  elseif (TSIMD_BENCHMARK_ISA STREQUAL "AVX512KNL")
    set(TSIMD_ISPC_TARGET_LIST avx512knl-i32x16)
  elseif (TSIMD_BENCHMARK_ISA STREQUAL "AVX2")
    set(TSIMD_ISPC_TARGET_LIST avx2)
  elseif (TSIMD_BENCHMARK_ISA STREQUAL "AVX")
    set(TSIMD_ISPC_TARGET_LIST avx)
  elseif (TSIMD_BENCHMARK_ISA STREQUAL "SSE4")
    set(TSIMD_ISPC_TARGET_LIST sse4)
  elseif (TSIMD_BENCHMARK_ISA STREQUAL "SCALAR")
    set(TSIMD_ISPC_TARGET_LIST sse2)
  else ()
    message(ERROR "Invalid TSIMD_BENCHMARK_ISA value. "
                  "Please select one of ${TSIMD_SUPPORTED_ISAS}")
  endif()
endmacro()

macro(ISPC_ADD_EXECUTABLE name)
  set(ISPC_SOURCES "")
  set(OTHER_SOURCES "")
  foreach(src ${ARGN})
    get_filename_component(ext ${src} EXT)
    if (ext STREQUAL ".ispc")
      if(TSIMD_ENABLE_ISPC_COMPARISONS)
        set(ISPC_SOURCES ${ISPC_SOURCES} ${src})
      endif()
    else ()
      set(OTHER_SOURCES ${OTHER_SOURCES} ${src})
    endif ()
  endforeach()
  tsimd_ispc_compile(${ISPC_SOURCES})
  add_executable(${name} ${ISPC_OBJECTS} ${OTHER_SOURCES} ${ISPC_SOURCES})
endmacro()
