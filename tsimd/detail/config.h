// ========================================================================== //
// The MIT License (MIT)                                                      //
//                                                                            //
// Copyright (c) 2017 Jefferson Amstutz                                       //
//                                                                            //
// Permission is hereby granted, free of charge, to any person obtaining a    //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,   //
// and/or sell copies of the Software, and to permit persons to whom the      //
// Software is furnished to do so, subject to the following conditions:       //
//                                                                            //
// The above copyright notice and this permission notice shall be included in //
// in all copies or substantial portions of the Software.                     //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    //
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        //
// DEALINGS IN THE SOFTWARE.                                                  //
// ========================================================================== //

#pragma once

#if defined(__AVX512F__)
#include <zmmintrin.h>
#define TSIMD_DEFAULT_WIDTH 16
#endif

#if defined(__AVX__)
#include <immintrin.h>
#if !defined(TSIMD_DEFAULT_WIDTH)
#define TSIMD_DEFAULT_WIDTH 8
#endif
#endif

#if defined(__SSE__)
#include <xmmintrin.h>
#if !defined(TSIMD_DEFAULT_WIDTH)
#define TSIMD_DEFAULT_WIDTH 4
#endif
#endif

#if !defined(TSIMD_DEFAULT_WIDTH)
#define TSIMD_DEFAULT_WIDTH 1
#endif

#if defined(__AVX512F__)
#define AVX_ZERO_UPPER()
#elif defined(__AVX__)
#define AVX_ZERO_UPPER() _mm256_zeroupper()
#else
#define AVX_ZERO_UPPER()
#endif

#ifdef _WIN32
#define TSIMD_ALIGN(...) __declspec(align(__VA_ARGS__))
#define TSIMD_INLINE inline
#else
#define TSIMD_ALIGN(...) __attribute__((aligned(__VA_ARGS__)))
#define TSIMD_INLINE inline __attribute__((always_inline))
#endif

#define NOT_YET_IMPLEMENTED \
  static_assert(false, "This function is not yet implemented!");

#define DO_NOT_USE \
  static_assert(false, "This function should not be used in this context!");
