// ========================================================================== //
// The MIT License (MIT)                                                      //
//                                                                            //
// Copyright (c) 2017 Intel Corporation                                       //
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

#ifdef _MSVC_LANG
#define TSIMD_WIN 1
#else
#define TSIMD_WIN 0
#endif

// NOTE(jda) - don't include intrinsics header if no vector ISA specified
#if defined(__AVX512F__) || defined(__AVX2__) || \
    defined(__AVX__) || defined(__SSE4_2__)
#include <immintrin.h>
#endif

#if defined(__AVX512F__)
#define TSIMD_DEFAULT_WIDTH 16
#endif

#if defined(__AVX2__) || defined(__AVX__)
#if !defined(TSIMD_DEFAULT_WIDTH)
#define TSIMD_DEFAULT_WIDTH 8
#endif
#endif

#if defined(__SSE4_2__)
#if !defined(TSIMD_DEFAULT_WIDTH)
#define TSIMD_DEFAULT_WIDTH 4
#endif
#endif

#if !defined(TSIMD_DEFAULT_WIDTH)
#define TSIMD_DEFAULT_WIDTH 1
#endif

#if defined(__AVX512F__)
#define AVX_ZERO_UPPER()
#elif defined(__AVX2__) || defined(__AVX__)
#define AVX_ZERO_UPPER() _mm256_zeroupper()
#else
#define AVX_ZERO_UPPER()
#endif

#if TSIMD_WIN
#define TSIMD_INLINE inline
#else
#define TSIMD_INLINE inline __attribute__((always_inline))
#endif

#if 0
#define NOT_YET_IMPLEMENTED \
  static_assert(false, "This function is not yet implemented!");
#elif 0
#define NOT_YET_IMPLEMENTED \
  throw std::runtime_error("This function is not yet implemented!");
#else
#define NOT_YET_IMPLEMENTED               \
  throw std::runtime_error(__FUNCTION__ + \
                           std::string(" is not yet implemented!"));
#endif

#define DO_NOT_USE \
  static_assert(false, "This function should not be used in this context!");

#if !defined(TSIMD_DEFAULT_NEAR_EQUAL_EPSILON)
#define TSIMD_DEFAULT_NEAR_EQUAL_EPSILON 1e-6f
#endif

#if defined(__INTEL_COMPILER)
# define TSIMD_COMPILER_INTEL 1
# define TSIMD_COMPILER_CLANG 0
# define TSIMD_COMPILER_GNU   0
# define TSIMD_COMPILER_MSVC  0
#elif defined(__clang__)
# define TSIMD_COMPILER_INTEL 0
# define TSIMD_COMPILER_CLANG 1
# define TSIMD_COMPILER_GNU   0
# define TSIMD_COMPILER_MSVC  0
#elif defined(__GNUC__) || defined(__GNUG__)
# define TSIMD_COMPILER_INTEL 0
# define TSIMD_COMPILER_CLANG 0
# define TSIMD_COMPILER_GNU   1
# define TSIMD_COMPILER_MSVC  0
#else
# define TSIMD_COMPILER_INTEL 0
# define TSIMD_COMPILER_CLANG 0
# define TSIMD_COMPILER_GNU   0
# define TSIMD_COMPILER_MSVC  1
#endif

namespace tsimd {
  namespace detail {

    // NOTE: only for identifying pack<> types at compile-time
    struct pack_base {};

  } // namespace detail
} // namespace tsimd
