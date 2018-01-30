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

#include "enable_if_t.h"
#include "simd_type.h"

namespace tsimd {
  namespace traits {

    // Provide a cast (float|int) intrinsic type given a SIMD width ///////////

    template <typename T>
    struct cast_simd_undefined_type
    {
    };

    template <typename T, int W>
    struct cast_simd_type
    {
      using type = cast_simd_undefined_type<T>;
    };

    // 1-wide //

    template <>
    struct cast_simd_type<float, 1>
    {
      using type = simd_type<int, 1>::type;
    };

    template <>
    struct cast_simd_type<int, 1>
    {
      using type = simd_type<float, 1>::type;
    };

    template <>
    struct cast_simd_type<double, 1>
    {
      using type = simd_type<long long, 1>::type;
    };

    template <>
    struct cast_simd_type<long long, 1>
    {
      using type = simd_type<double, 1>::type;
    };

    // 4-wide //

    template <>
    struct cast_simd_type<float, 4>
    {
      using type = simd_type<int, 4>::type;
    };

    template <>
    struct cast_simd_type<int, 4>
    {
      using type = simd_type<float, 4>::type;
    };

    template <>
    struct cast_simd_type<bool32_t, 4>
    {
      using type = simd_type<int, 4>::type;
    };

    // 8-wide //

    template <>
    struct cast_simd_type<float, 8>
    {
      using type = simd_type<int, 8>::type;
    };

    template <>
    struct cast_simd_type<int, 8>
    {
      using type = simd_type<float, 8>::type;
    };

    template <>
    struct cast_simd_type<bool32_t, 8>
    {
      using type = simd_type<int, 8>::type;
    };

    // 16-wide //

    template <>
    struct cast_simd_type<float, 16>
    {
      using type = simd_type<int, 16>::type;
    };

    template <>
    struct cast_simd_type<int, 16>
    {
      using type = simd_type<float, 16>::type;
    };

  }  // namespace traits
}  // namespace tsimd