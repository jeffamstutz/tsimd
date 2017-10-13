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
#include "tsimd_traits.h"

namespace tsimd {

  template <typename T, int W = DEFAULT_WIDTH>
  struct pack
  {
    pack() = default;
    pack(T value);

    const T& operator[](int i) const;
          T& operator[](int i);

    template <typename OTHER_T>
    pack<OTHER_T, W> as();

    // Compile-time info //

    enum {static_size = W};
    using type = T;

    // Data //

    using INTRINSIC_T = typename traits::simd_type_from_width<T, W>::type;

    union {
      T data[W];
      INTRINSIC_T v;
    };

    // Interface checks //

    static_assert(W == 1 || W == 4 || W == 8 || W == 16,
                  "SIMD width must be 1, 4, 8, or 16!");
  };

  template <int W = DEFAULT_WIDTH>
  using mask = pack<int, W>;

  // pack<> inlined members ///////////////////////////////////////////////////

  template <typename T, int W>
  inline pack<T, W>::pack(T value)
  {
    #pragma omp simd
    for(int i = 0; i < W; ++i)
      data[i] = value;
  }

  template <typename T, int W>
  inline const T& pack<T, W>::operator[](int i) const
  {
    return data[i];
  }

  template <typename T, int W>
  inline T& pack<T, W>::operator[](int i)
  {
    return data[i];
  }

  template <typename T, int W>
  template <typename OTHER_T>
  inline pack<OTHER_T, W> pack<T, W>::as()
  {
    pack<OTHER_T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = data[i];

    return result;
  }

} // ::tsimd