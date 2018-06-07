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

#include "../utility/bool_t.h"

namespace tsimd {
  namespace traits {

    // Provide appropriate underlying array type for given T, W ///////////////

    template <typename T, int W>
    struct array_for_pack
    {
      using type = std::array<T, W>;
    };

#if defined(__AVX512F__)
    template <>
    struct array_for_pack<bool64_t, 8>
    {
      using type = std::array<__mmask8, 1>;
    };

    template <>
    struct array_for_pack<bool32_t, 16>
    {
      using type = std::array<__mmask8, 2>;
    };

    template <>
    struct array_for_pack<bool64_t, 16>
    {
      using type = std::array<__mmask8, 2>;
    };
#endif

#if defined(__AVX512VL__)
    template <>
    struct array_for_pack<bool32_t, 8>
    {
      using type = std::array<__mmask8, 1>;
    };
#endif

    // Half-sized array ///////////////////////////////////////////////////////

    template <typename T, int W>
    struct half_array_for_pack
    {
      using type = std::array<T, W / 2>;
    };

#if defined(__AVX512F__)
    template <>
    struct half_array_for_pack<bool64_t, 8>
    {
      using type = std::array<__mmask8, 1>;
    };

    template <>
    struct half_array_for_pack<bool32_t, 16>
    {
      using type = std::array<__mmask8, 1>;
    };

    template <>
    struct half_array_for_pack<bool64_t, 16>
    {
      using type = std::array<__mmask8, 1>;
    };
#endif

  }  // namespace traits
}  // namespace tsimd
