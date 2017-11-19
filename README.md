# tsimd
Fundamental C++ SIMD types for Intel CPUs (sse, avx, avx2, avx512)

Master Status: [![Build Status](https://travis-ci.org/jeffamstutz/tsimd.svg?branch=master)](https://travis-ci.org/jeffamstutz/tsimd)

TODO:

- SIMD arithmetic functions
- SSE + AVX512 implementations (i.e. 4 + 16 wide pack<>)
- 64-bit element pack of widths > 1
- replace ```#pragma omp simd``` loop implementations with intrinsics where appropriate
