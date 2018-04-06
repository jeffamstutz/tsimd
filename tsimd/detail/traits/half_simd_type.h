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

#include <array>

#include "enable_if_t.h"
#include "simd_type.h"

namespace tsimd {
  namespace traits {

    // Provide intrinsic type half the size of given width ////////////////////

    template <typename T, int W>
    struct half_simd_type
    {
      using type = std::array<T, W / 2>;
      static const bool is_array = true;
    };

    // 1-wide //

    template <typename T>
    struct half_simd_type<T, 1>
    {
      using type = std::array<T, 1>;
      static const bool is_array = true;
    };

    // 8-wide //

#if defined(__SSE4_2__)
    template <>
    struct half_simd_type<float, 8>
    {
      using type = simd_type<float, 4>::type;
      static const bool is_array = false;
    };

    template <>
    struct half_simd_type<int, 8>
    {
      using type = simd_type<int, 4>::type;
      static const bool is_array = false;
    };

    template <>
    struct half_simd_type<bool32_t, 8>
    {
      using type = half_simd_type<float, 8>::type;
      static const bool is_array = false;
    };
#endif

    // 16-wide //

#if defined(__AVX2__) || defined(__AVX__)
    template <>
    struct half_simd_type<float, 16>
    {
      using type = simd_type<float, 8>::type;
      static const bool is_array = false;
    };
#endif

#if defined(__AVX2__)
    template <>
    struct half_simd_type<int, 16>
    {
      using type = simd_type<int, 8>::type;
      static const bool is_array = false;
    };
#endif

#if defined(__AVX512F__)
    template <>
    struct half_simd_type<bool32_t, 16>
    {
      using type = typename simd_type<bool32_t, 8>::type;
      static const bool is_array = false;
    };

    template <>
    struct half_simd_type<bool64_t, 16>
    {
      using type = typename simd_type<bool64_t, 8>::type;
      static const bool is_array = false;
    };
#endif

    // Provide half_simd_type is a std::array<T, W/2> /////////////////////////

    template <typename T, int W>
    struct half_simd_is_array
    {
      static const bool value = half_simd_type<T, W>::is_array;
    };

  }  // namespace traits
}  // namespace tsimd
