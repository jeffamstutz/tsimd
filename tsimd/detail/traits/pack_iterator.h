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

#include "../config.h"
#include "../utility/bit_iterator.h"

namespace tsimd {
  namespace traits {

    // Return the type of iterator for pack<>, default is a T* ////////////////

    template <typename T, int W>
    struct pack_iterator
    {
      using type = T*;
    };

#if defined(__AVX512VL__)
    template <>
    struct pack_iterator<bool32_t, 8>
    {
      using type = bit_iterator;
    };
#endif


#if defined(__AVX512F__)
    template <>
    struct pack_iterator<bool64_t, 8>
    {
      using type = bit_iterator;
    };

    template <>
    struct pack_iterator<bool32_t, 16>
    {
      using type = bit_iterator;
    };

    template <>
    struct pack_iterator<bool64_t, 16>
    {
      using type = bit_iterator;
    };
#endif

    template <typename T, int W>
    struct const_pack_iterator
    {
      using type = const T*;
    };

#if defined(__AVX512VL__)
    template <>
    struct const_pack_iterator<bool32_t, 8>
    {
      using type = bit_iterator;
    };
#endif

#if defined(__AVX512F__)
    template <>
    struct const_pack_iterator<bool64_t, 8>
    {
      using type = bit_iterator;
    };

    template <>
    struct const_pack_iterator<bool32_t, 16>
    {
      using type = bit_iterator;
    };

    template <>
    struct const_pack_iterator<bool64_t, 16>
    {
      using type = bit_iterator;
    };
#endif

  }  // namespace traits
}  // namespace tsimd
