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
#include "enable_if_t.h"
#include "simd_type.h"

namespace tsimd {
  namespace traits {

    // Bool type for given primitive type /////////////////////////////////////

    template <typename T>
    struct bool_undefined_type
    {
    };

    template <typename T>
    struct bool_type_for
    {
      using type = bool_undefined_type<T>;
    };

    // 32-bit //

    template <>
    struct bool_type_for<float>
    {
      using type = bool32_t;
    };

    template <>
    struct bool_type_for<int>
    {
      using type = bool32_t;
    };

    template <>
    struct bool_type_for<bool32_t>
    {
      using type = bool32_t;
    };

    // 64-bit //

    template <>
    struct bool_type_for<double>
    {
      using type = bool64_t;
    };

    template <>
    struct bool_type_for<long long>
    {
      using type = bool64_t;
    };

    template <>
    struct bool_type_for<bool64_t>
    {
      using type = bool64_t;
    };

  }  // namespace traits
}  // namespace tsimd
