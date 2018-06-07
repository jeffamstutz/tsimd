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

#include "../utility/bool_t.h"
#include "array_for_pack.h"
#include "enable_if_t.h"

namespace tsimd {
  namespace traits {

    // Provide intrinsic type given a SIMD width //////////////////////////////

    template <typename T, int W>
    struct simd_undefined_type
    {
    };

    template <typename T, int W>
    struct simd_type
    {
      using type = simd_undefined_type<T, W>;
    };

    // bool types all widths //

    template <int W>
    struct simd_type<bool32_t, W>
    {
      using type = typename simd_type<float, W>::type;
    };

    template <int W>
    struct simd_type<bool64_t, W>
    {
      using type = typename simd_type<double, W>::type;
    };

    // 1-wide //

    template <>
    struct simd_type<float, 1>
    {
      using type = float;
    };

    template <>
    struct simd_type<int, 1>
    {
      using type = int;
    };

    template <>
    struct simd_type<double, 1>
    {
      using type = double;
    };

    template <>
    struct simd_type<long long, 1>
    {
      using type = long long;
    };

    // 4-wide //

#if defined(__SSE4_2__)
    template <>
    struct simd_type<float, 4>
    {
      using type = __m128;
    };

    template <>
    struct simd_type<int, 4>
    {
      using type = __m128i;
    };
#endif

#if defined(__AVX2__) || defined(__AVX__)
    template <>
    struct simd_type<double, 4>
    {
      using type = __m256d;
    };
#endif

#if defined(__AVX2__)
    template <>
    struct simd_type<long long, 4>
    {
      using type = __m256i;
    };
#endif

    // 8-wide //

#if defined(__AVX2__) || defined(__AVX__)
    template <>
    struct simd_type<float, 8>
    {
      using type = __m256;
    };

    template <>
    struct simd_type<int, 8>
    {
      using type = __m256i;
    };
#endif

#if defined(__AVX512F__)
    template <>
    struct simd_type<double, 8>
    {
      using type = __m512d;
    };

    template <>
    struct simd_type<long long, 8>
    {
      using type = __m512i;
    };

    template <>
    struct simd_type<bool64_t, 8>
    {
      using type = __mmask8;
    };
#endif

#if defined(__AVX512VL__)
    template <>
    struct simd_type<bool32_t, 8>
    {
      using type = __mmask8;
    };
#endif

    // 16-wide //

#if defined(__AVX512F__)
    template <>
    struct simd_type<float, 16>
    {
      using type = __m512;
    };

    template <>
    struct simd_type<int, 16>
    {
      using type = __m512i;
    };

    template <>
    struct simd_type<bool32_t, 16>
    {
      using type = __mmask16;
    };

    template <>
    struct simd_type<bool64_t, 16>
    {
      using type = __mmask16;
    };
#endif

    // Check if a pack<T, W>::intrinsic_t is actually an intrinsic type ///////

    // example: pack<float, 8> on AVX is type __m256, so evaluate to "true"

    template <typename T, int W>
    using simd_type_is_native =
        std::is_same<simd_type<T,W>, simd_undefined_type<T,W>>;

    template <typename T, int W>
    using simd_type_is_native_t =
        enable_if_t<simd_type_is_native<T,W>::value>;

    template <typename T, int W>
    using simd_type_is_not_native_t =
        enable_if_t<!simd_type_is_native<T,W>::value>;

    // Return the native SIMD type (e.g. _m256) if availab, otherwise arr /////

    // NOTE(jda) - These are for copy/move constructors and assignment operators
    //             so the optimal copy type is inferred based on the target ISA
    //             and pack<> type/size combination.

    template <typename T, int W>
    struct simd_or_array_type
    {
      using type = typename array_for_pack<T, W>::type;
    };

    // 4-wide //

#if defined(__SSE4_2__)
    template <>
    struct simd_or_array_type<float, 4>
    {
      using type = typename simd_type<float, 4>::type;
    };

    template <>
    struct simd_or_array_type<int, 4>
    {
      using type = typename simd_type<int, 4>::type;
    };
#endif

#if defined(__AVX2__) || defined(__AVX__)
    template <>
    struct simd_or_array_type<double, 4>
    {
      using type = typename simd_type<double, 4>::type;
    };
#endif

#if defined(__AVX2__)
    template <>
    struct simd_or_array_type<long long, 4>
    {
      using type = typename simd_type<long long, 4>::type;
    };
#endif

    // 8-wide //

#if defined(__AVX2__) || defined(__AVX__)
    template <>
    struct simd_or_array_type<float, 8>
    {
      using type = typename simd_type<float, 8>::type;
    };

    template <>
    struct simd_or_array_type<int, 8>
    {
      using type = typename simd_type<int, 8>::type;
    };

    template <>
    struct simd_or_array_type<bool32_t, 8>
    {
      using type = typename simd_type<bool32_t, 8>::type;
    };
#endif

#if defined(__AVX512F__)
    template <>
    struct simd_or_array_type<double, 8>
    {
      using type = typename simd_type<double, 8>::type;
    };

    template <>
    struct simd_or_array_type<long long, 8>
    {
      using type = typename simd_type<long long, 8>::type;
    };

    template <>
    struct simd_or_array_type<bool64_t, 8>
    {
      using type = typename simd_type<bool64_t, 8>::type;
    };
#endif

    // 16-wide //

#if defined(__AVX512F__)
    template <>
    struct simd_or_array_type<float, 16>
    {
      using type = typename simd_type<float, 16>::type;
    };

    template <>
    struct simd_or_array_type<int, 16>
    {
      using type = typename simd_type<int, 16>::type;
    };

    template <>
    struct simd_or_array_type<bool32_t, 16>
    {
      using type = typename simd_type<bool32_t, 16>::type;
    };

    template <>
    struct simd_or_array_type<bool64_t, 16>
    {
      using type = typename simd_type<bool64_t, 16>::type;
    };
#endif

  }  // namespace traits
}  // namespace tsimd
