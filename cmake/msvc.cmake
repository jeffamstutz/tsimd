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

SET(FLAGS_SSE2  "/D__SSE__ /D__SSE2__")
SET(FLAGS_SSE42 "${FLAGS_SSE2} /D__SSE3__ /D__SSSE3__ /D__SSE4_1__ /D__SSE4_2__")
SET(FLAGS_AVX   "${FLAGS_SSE42} /arch:AVX")
SET(FLAGS_AVX2  "${FLAGS_SSE42} /arch:AVX2")

# TODO: SKX?

if (TSIMD_BENCHMARK_ISA STREQUAL "AVX2")
  set (ISA_FLAGS ${FLAGS_AVX2})
elseif (TSIMD_BENCHMARK_ISA STREQUAL "AVX")
  set (ISA_FLAGS ${FLAGS_AVX})
elseif (TSIMD_BENCHMARK_ISA STREQUAL "SSE4")
  set (ISA_FLAGS ${FLAGS_SSE42})
endif()

set(CMAKE_CXX_FLAGS "${ISA_FLAGS} ${CMAKE_CXX_FLAGS}")
