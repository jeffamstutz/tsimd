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
SET(ISPC_VERSION_WORKING "1.9.1")
LIST(GET ISPC_VERSION_WORKING -1 ISPC_VERSION_REQUIRED)

IF (NOT ISPC_EXECUTABLE)
  # try sibling folder as hint for path of ISPC
  IF (APPLE)
    SET(ISPC_DIR_SUFFIX "osx")
  ELSEIF(WIN32)
    SET(ISPC_DIR_SUFFIX "windows")
    IF (MSVC_VERSION LESS 1900)
      LIST(APPEND ISPC_DIR_SUFFIX "windows-vs2013")
    ELSE()
      LIST(APPEND ISPC_DIR_SUFFIX "windows-vs2015")
    ENDIF()
  ELSE()
    SET(ISPC_DIR_SUFFIX "linux")
  ENDIF()
  FOREACH(ver ${ISPC_VERSION_WORKING})
    FOREACH(suffix ${ISPC_DIR_SUFFIX})
      LIST(APPEND ISPC_DIR_HINT ${PROJECT_SOURCE_DIR}/../ispc-v${ver}-${suffix})
    ENDFOREACH()
  ENDFOREACH()

  FIND_PROGRAM(ISPC_EXECUTABLE ispc HINTS ${ISPC_DIR_HINT} DOC "Path to the ISPC executable.")
  IF (NOT ISPC_EXECUTABLE)
    MESSAGE(STATUS "Could not find ISPC...disabling ISPC examples")
    SET(TSIMD_ENABLE_ISPC OFF)
  ELSE()
    MESSAGE(STATUS "Found Intel SPMD Compiler (ISPC): ${ISPC_EXECUTABLE}")
  ENDIF()
ENDIF()

IF(TSIMD_ENABLE_ISPC)
  IF(NOT ISPC_VERSION)
    EXECUTE_PROCESS(COMMAND ${ISPC_EXECUTABLE} --version OUTPUT_VARIABLE ISPC_OUTPUT)
    STRING(REGEX MATCH " ([0-9]+[.][0-9]+[.][0-9]+)(dev|knl)? " DUMMY "${ISPC_OUTPUT}")
    SET(ISPC_VERSION ${CMAKE_MATCH_1})

    IF (ISPC_VERSION VERSION_LESS ISPC_VERSION_REQUIRED)
      MESSAGE(FATAL_ERROR "Need at least version ${ISPC_VERSION_REQUIRED} of Intel SPMD Compiler (ISPC).")
    ENDIF()

    SET(ISPC_VERSION ${ISPC_VERSION} CACHE STRING "ISPC Version")
    MARK_AS_ADVANCED(ISPC_VERSION)
    MARK_AS_ADVANCED(ISPC_EXECUTABLE)
  ENDIF()

  GET_FILENAME_COMPONENT(ISPC_DIR ${ISPC_EXECUTABLE} PATH)
ENDIF()

# ##################################################################
# add macro INCLUDE_DIRECTORIES_ISPC() that allows to specify search
# paths for ISPC sources
# ##################################################################
SET(ISPC_INCLUDE_DIR "")
MACRO (INCLUDE_DIRECTORIES_ISPC)
  SET(ISPC_INCLUDE_DIR ${ISPC_INCLUDE_DIR} ${ARGN})
ENDMACRO ()

MACRO (TSIMD_ISPC_COMPILE)
  SET(ISPC_ADDITIONAL_ARGS "")
  SET(ISPC_TARGETS ${TSIMD_ISPC_TARGET_LIST})

  SET(ISPC_TARGET_EXT ${CMAKE_CXX_OUTPUT_EXTENSION})
  STRING(REPLACE ";" "," ISPC_TARGET_ARGS "${ISPC_TARGETS}")

  IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(ISPC_ARCHITECTURE "x86-64")
  ELSE()
    SET(ISPC_ARCHITECTURE "x86")
  ENDIF()

  SET(ISPC_TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR})
  INCLUDE_DIRECTORIES(${ISPC_TARGET_DIR})

  IF(ISPC_INCLUDE_DIR)
    STRING(REPLACE ";" ";-I;" ISPC_INCLUDE_DIR_PARMS "${ISPC_INCLUDE_DIR}")
    SET(ISPC_INCLUDE_DIR_PARMS "-I" ${ISPC_INCLUDE_DIR_PARMS})
  ENDIF()

  IF (WIN32 OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    SET(ISPC_OPT_FLAGS -O3)
  ELSE()
    SET(ISPC_OPT_FLAGS -O2 -g)
  ENDIF()

  IF (NOT WIN32)
    SET(ISPC_ADDITIONAL_ARGS ${ISPC_ADDITIONAL_ARGS} --pic)
  ENDIF()

  IF (NOT TSIMD_DEBUG_BUILD)
    SET(ISPC_ADDITIONAL_ARGS ${ISPC_ADDITIONAL_ARGS} --opt=disable-assertions)
  ENDIF()

  SET(ISPC_OBJECTS "")

  FOREACH(src ${ARGN})
    GET_FILENAME_COMPONENT(fname ${src} NAME_WE)
    GET_FILENAME_COMPONENT(dir ${src} PATH)

    SET(input ${src})
    IF ("${dir}" MATCHES "^/") # absolute unix-style path to input
      SET(outdir "${ISPC_TARGET_DIR}/rebased${dir}")
    ELSEIF ("${dir}" MATCHES "^[A-Z]:") # absolute DOS-style path to input
      STRING(REGEX REPLACE "^[A-Z]:" "${ISPC_TARGET_DIR}/rebased/" outdir "${dir}")
    ELSE() # relative path to input
      SET(outdir "${ISPC_TARGET_DIR}/local_${dir}")
      SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${src})
    ENDIF()

    SET(deps "")
    IF (EXISTS ${outdir}/${fname}.dev.idep)
      FILE(READ ${outdir}/${fname}.dev.idep contents)
      STRING(REPLACE " " ";"     contents "${contents}")
      STRING(REPLACE ";" "\\\\;" contents "${contents}")
      STRING(REPLACE "\n" ";"    contents "${contents}")
      FOREACH(dep ${contents})
        IF (EXISTS ${dep})
          SET(deps ${deps} ${dep})
        ENDIF (EXISTS ${dep})
      ENDFOREACH(dep ${contents})
    ENDIF ()

    SET(results "${outdir}/${fname}.dev${ISPC_TARGET_EXT}")

    # if we have multiple targets add additional object files
    LIST(LENGTH ISPC_TARGETS NUM_TARGETS)
    IF (NUM_TARGETS GREATER 1)
      FOREACH(target ${ISPC_TARGETS})
        STRING(REPLACE "-i32x16" "" target ${target}) # strip avx512(knl|skx)-i32x16
        SET(results ${results} "${outdir}/${fname}.dev_${target}${ISPC_TARGET_EXT}")
      ENDFOREACH()
    ENDIF()

    ADD_CUSTOM_COMMAND(
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

    SET(ISPC_OBJECTS ${ISPC_OBJECTS} ${results})
  ENDFOREACH()
ENDMACRO()

## Macro configure ISA targets for ispc ##
MACRO(TSIMD_CONFIGURE_ISPC_ISA)
  SET(TSIMD_BUILD_ISA "AVX" CACHE STRING
      "Target ISA (SSE4, AVX, AVX2, AVX512KNL, AVX512SKX)")
  STRING(TOUPPER ${TSIMD_BUILD_ISA} TSIMD_BUILD_ISA)
  SET_PROPERTY(CACHE TSIMD_BUILD_ISA PROPERTY STRINGS
               SSE4 AVX AVX2 AVX512KNL AVX512SKX)

  UNSET(TSIMD_ISPC_TARGET_LIST)

  IF (TSIMD_BUILD_ISA STREQUAL "AVX512SKX")
    SET(TSIMD_ISPC_TARGET_LIST avx512skx-i32x16)
  ELSEIF (TSIMD_BUILD_ISA STREQUAL "AVX512KNL")
    SET(TSIMD_ISPC_TARGET_LIST avx512knl-i32x16)
  ELSEIF (TSIMD_BUILD_ISA STREQUAL "AVX2")
    SET(TSIMD_ISPC_TARGET_LIST avx2)
  ELSEIF (TSIMD_BUILD_ISA STREQUAL "AVX")
    SET(TSIMD_ISPC_TARGET_LIST avx)
  ELSEIF (TSIMD_BUILD_ISA STREQUAL "SSE4")
    SET(TSIMD_ISPC_TARGET_LIST sse4)
  ELSE ()
    MESSAGE(ERROR "Invalid TSIMD_BUILD_ISA value. "
                  "Please select one of ${TSIMD_SUPPORTED_ISAS}")
  ENDIF()
ENDMACRO()

MACRO(ISPC_ADD_EXECUTABLE name)
  SET(ISPC_SOURCES "")
  SET(OTHER_SOURCES "")
  FOREACH(src ${ARGN})
    GET_FILENAME_COMPONENT(ext ${src} EXT)
    IF (ext STREQUAL ".ispc")
      IF(TSIMD_ENABLE_ISPC)
        SET(ISPC_SOURCES ${ISPC_SOURCES} ${src})
      ENDIF()
    ELSE ()
      SET(OTHER_SOURCES ${OTHER_SOURCES} ${src})
    ENDIF ()
  ENDFOREACH()
  TSIMD_ISPC_COMPILE(${ISPC_SOURCES})
  ADD_EXECUTABLE(${name} ${ISPC_OBJECTS} ${OTHER_SOURCES} ${ISPC_SOURCES})
ENDMACRO()