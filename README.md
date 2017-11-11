# tsimd
Fundamental C++ SIMD types for Intel CPUs (sse, avx, avx2, avx512)

Master Status: [![Build Status](https://travis-ci.org/jeffamstutz/tsimd.svg?branch=master)](https://travis-ci.org/jeffamstutz/tsimd)

TODO:

- 64 bit element types in pack<> (i.e. ```double``` and ```long long```)
- 32 vs. 64 bit element masks
- SIMD arithmetic functions
- SSE + AVX512 implementations (i.e. 4 + 16 wide pack<>)
- replace ```#pragma omp simd``` loop implementations with intrinsics where appropriate
- make all correctness tests independant of pack<> size and build each version
