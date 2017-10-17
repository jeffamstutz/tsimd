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

#include "config.h"

namespace tsimd {
  namespace traits {

    // C++14 traits for C++11 /////////////////////////////////////////////////

    template <bool B, class T = void>
    using enable_if_t = typename std::enable_if<B, T>::type;

    // Provide intrinsic type given a SIMD width //////////////////////////////

    template <typename T, int W>
    struct simd_type
    {
      using type = void;  // NOTE(jda) - use 'void' to flag a <T,W> pair which
                          //             is missing
    };

    // 1-wide //

    template <typename T>
    struct simd_type<T, 1>
    {
      using type = T;
    };

    // 4-wide //

    template <>
    struct simd_type<float, 4>
    {
#if defined(__SSE__)
      using type = __m128;
#else
      using type = float[4];
#endif
    };

    template <>
    struct simd_type<int, 4>
    {
#if defined(__SSE__)
      using type = __m128i;
#else
      using type = int[4];
#endif
    };

    // 8-wide //

    template <>
    struct simd_type<float, 8>
    {
#if defined(__AVX__) || defined(__AVX512__)
      using type = __m256;
#elif defined(__SSE__)
      using type = __m128[2];
#else
      using type = float[8];
#endif
    };

    template <>
    struct simd_type<int, 8>
    {
#if defined(__AVX__) || defined(__AVX512__)
      using type = __m256i;
#elif defined(__SSE__)
      using type = __m128i[2];
#else
      using type = int[8];
#endif
    };

    // 16-wide //

    // TODO

    // Provide intrinsic type half the size of given width ////////////////////

    template <typename T, int W>
    struct half_simd_type
    {
      using type = void;  // NOTE(jda) - use 'void' to flag a <T,W> pair which
                          //             is missing
    };

    // 1-wide //

    template <>
    struct half_simd_type<int, 1>  // NOTE: no float equivalent!
    {
      using type = short;
    };

    // 4-wide //

    template <>
    struct half_simd_type<float, 4>
    {
      using type = float[2];
    };

    template <>
    struct half_simd_type<int, 4>
    {
      using type = int[2];
    };

    // 8-wide //

    template <>
    struct half_simd_type<float, 8>
    {
#if defined(__AVX__) || defined(__AVX512__) || defined(__SSE__)
      using type = __m128;
#else
      using type = float[4];
#endif
    };

    template <>
    struct half_simd_type<int, 8>
    {
#if defined(__AVX__) || defined(__AVX512__) || defined(__SSE__)
      using type = __m128i;
#else
      using type = int[4];
#endif
    };

    // 16-wide //

    // TODO

    // If a single type is convertable to another /////////////////////////////

    template <typename FROM, typename TO>
    using can_convert = enable_if_t<std::is_convertible<TO, FROM>::value>;

  }  // namespace traits
}  // namespace tsimd