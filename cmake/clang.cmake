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

set(FLAGS_SSE42 "-msse4.2")
set(FLAGS_AVX   "-mavx")
set(FLAGS_AVX2  "-mf16c -mavx2 -mfma -mlzcnt -mbmi -mbmi2")
set(FLAGS_AVX512KNL "-march=knl")
set(FLAGS_AVX512SKX "-march=skx")

if (TSIMD_BENCHMARK_ISA STREQUAL "NATIVE")
  set (ISA_FLAGS "-march=native")
elseif (TSIMD_BENCHMARK_ISA STREQUAL "AVX512SKX")
  set (ISA_FLAGS ${FLAGS_AVX512SKX})
elseif (TSIMD_BENCHMARK_ISA STREQUAL "AVX512KNL")
  set (ISA_FLAGS ${FLAGS_AVX512KNL})
elseif (TSIMD_BENCHMARK_ISA STREQUAL "AVX2")
  set (ISA_FLAGS ${FLAGS_AVX2})
elseif (TSIMD_BENCHMARK_ISA STREQUAL "AVX")
  set (ISA_FLAGS ${FLAGS_AVX})
elseif (TSIMD_BENCHMARK_ISA STREQUAL "SSE4")
  set (ISA_FLAGS ${FLAGS_SSE42})
endif()

SET(CMAKE_CXX_FLAGS "-std=c++11 -fno-strict-aliasing ${ISA_FLAGS} ${CMAKE_CXX_FLAGS}")

if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++") # link against C++11 stdlib
endif()
