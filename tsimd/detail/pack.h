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
    // Compile-time info //

    enum {static_size = W};
    using value_t = T;
    using intrinsic_t = typename traits::simd_type_from_width<T, W>::type;

    // Construction //

    pack() = default;
    pack(value_t value);
    pack(intrinsic_t value);

    // Array access //

    const T& operator[](int i) const;
          T& operator[](int i);

    // Cast //

    template <typename OTHER_T>
    pack<OTHER_T, W> as();

    // Data conversions //

    operator const intrinsic_t &() const { return v; }
    operator       intrinsic_t &()       { return v; }

    // Data //

    union
    {
      value_t data[W];
      intrinsic_t v;
    };

    // Interface checks //

    static_assert(W == 1 || W == 4 || W == 8 || W == 16,
                  "SIMD width must be 1, 4, 8, or 16!");
  };

  /* 1-wide shortcuts */
  using vfloat1  = pack<float, 1>;
  using vdouble1 = pack<double, 1>;
  using vint1    = pack<int, 1>;
  using vuint1   = pack<unsigned int, 1>;
  using vllong1  = pack<long long, 1>;
  using vbool1   = pack<int, 1>;
  using vboolf1  = vbool1;
  using vboold1  = vllong1;

  /* 4-wide shortcuts */
  using vfloat4  = pack<float, 4>;
  using vdouble4 = pack<double, 4>;
  using vint4    = pack<int, 4>;
  using vuint4   = pack<unsigned int, 4>;
  using vllong4  = pack<long long, 4>;
  using vbool4   = pack<int, 4>;
  using vboolf4  = vbool4;
  using vboold4  = vllong4;

  /* 8-wide shortcuts */
  using vfloat8  = pack<float, 8>;
  using vdouble8 = pack<double, 8>;
  using vint8    = pack<int, 8>;
  using vuint8   = pack<unsigned int, 8>;
  using vllong8  = pack<long long, 8>;
  using vbool8   = pack<int, 8>;
  using vboolf8  = vbool8;
  using vboold8  = vllong8;

  /* 16-wide shortcuts */
  using vfloat16  = pack<float, 16>;
  using vdouble16 = pack<double, 16>;
  using vint16    = pack<int, 16>;
  using vuint16   = pack<unsigned int, 16>;
  using vllong16  = pack<long long, 16>;
  using vbool16   = pack<int, 16>;
  using vboolf16  = vbool16;
  using vboold16  = vllong16;

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
  inline pack<T, W>::pack(pack<T, W>::intrinsic_t value)
  {
    v = value;
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