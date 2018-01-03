# tsimd - Fundamental C++ SIMD types for Intel CPUs (sse, avx, avx2, avx512)

This library is header-only and is implemented according to which Intel ISA
flags are enabled in the translation unit for which they are used (e.g. -mavx
with gcc or clang).

Master Status: [![Build Status](https://travis-ci.org/jeffamstutz/tsimd.svg?branch=master)](https://travis-ci.org/jeffamstutz/tsimd)

Requirements:

- C++11 (that's it!)

TODO:

- 64-bit element pack of widths > 1
- replace ```#pragma omp simd``` loop implementations with intrinsics where
  appropriate
