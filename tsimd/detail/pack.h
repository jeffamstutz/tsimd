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
    using value_t = typename std::decay<T>::type;
    using intrinsic_t = typename traits::simd_type<T, W>::type;
    using half_intrinsic_t = typename traits::half_simd_type<T, W>::type;

    // Construction //

    pack() = default;
    pack(T value); // NOTE(jda) - this should probable be marked 'explicit'...
    pack(intrinsic_t value);

#if 0 // NOTE: can this be culled for types that don't make sense?
    template <int, typename = traits::enable_if_t<W == 8>>
#endif
    pack(half_intrinsic_t a, half_intrinsic_t b) : vl(a), vh(b) {}

    // Array access //

    const T& operator[](int i) const;
          T& operator[](int i);

    // Cast //

    template <typename OTHER_T>
    pack<OTHER_T, W> as();

    // Data conversions //

    operator const intrinsic_t &() const { return v; }
    operator       intrinsic_t &()       { return v; }

    operator const T*() const { return data; }
    operator       T*()       { return data; }

    T* begin();
    T* end();

    const T* cbegin() const;
    const T* cend()   const;

    // Data //

    union
    {
      T data[W];
      intrinsic_t v;
      struct { half_intrinsic_t vl,vh; };
    };

    // Interface checks //

    static_assert(W == 1 || W == 4 || W == 8 || W == 16,
                  "SIMD width must be 1, 4, 8, or 16!");
  };

  // mask types and true/false value aliases //

  template <int W = DEFAULT_WIDTH>
  using mask = pack<float, W>;

  using mask_t = float;

  static const auto vtrue_v  = 0xFFFFFFFF;
  static const auto vfalse_v = 0x00000000;

  static const mask_t vtrue  = reinterpret_cast<const mask_t&>(vtrue_v);
  static const mask_t vfalse = reinterpret_cast<const mask_t&>(vfalse_v);

  /* 1-wide shortcuts */
  using vfloat1  = pack<float, 1>;
  using vdouble1 = pack<double, 1>;
  using vint1    = pack<int, 1>;
  using vuint1   = pack<unsigned int, 1>;
  using vllong1  = pack<long long, 1>;
  using vboolf1  = pack<mask_t, 1>;
  using vbool1   = vboolf1;
  using vboold1  = vllong1;

  /* 4-wide shortcuts */
  using vfloat4  = pack<float, 4>;
  using vdouble4 = pack<double, 4>;
  using vint4    = pack<int, 4>;
  using vuint4   = pack<unsigned int, 4>;
  using vllong4  = pack<long long, 4>;
  using vboolf4  = pack<mask_t, 4>;
  using vbool4   = vboolf4;
  using vboold4  = vllong4;

  /* 8-wide shortcuts */
  using vfloat8  = pack<float, 8>;
  using vdouble8 = pack<double, 8>;
  using vint8    = pack<int, 8>;
  using vuint8   = pack<unsigned int, 8>;
  using vllong8  = pack<long long, 8>;
  using vboolf8  = pack<mask_t, 8>;
  using vbool8   = vboolf8;
  using vboold8  = vllong8;

  /* 16-wide shortcuts */
  using vfloat16  = pack<float, 16>;
  using vdouble16 = pack<double, 16>;
  using vint16    = pack<int, 16>;
  using vuint16   = pack<unsigned int, 16>;
  using vllong16  = pack<long long, 16>;
  using vboolf16  = pack<mask_t, 16>;
  using vbool16   = vboolf16;
  using vboold16  = vllong16;

  /* default shortcuts */
  using vfloat  = pack<float, DEFAULT_WIDTH>;
  using vdouble = pack<double, DEFAULT_WIDTH>;
  using vint    = pack<int, DEFAULT_WIDTH>;
  using vuint   = pack<unsigned int, DEFAULT_WIDTH/2>;
  using vllong  = pack<long long, DEFAULT_WIDTH/2>;
  using vbool   = pack<mask_t, DEFAULT_WIDTH>;
  using vboolf  = vfloat;
  using vboold  = vllong;

  // pack<> TSIMD_INLINEd members ///////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(T value)
  {
    #pragma omp simd
    for(int i = 0; i < W; ++i)
      data[i] = value;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(pack<T, W>::intrinsic_t value)
  {
    v = value;
  }

  template <typename T, int W>
  TSIMD_INLINE const T& pack<T, W>::operator[](int i) const
  {
    return data[i];
  }

  template <typename T, int W>
  TSIMD_INLINE T& pack<T, W>::operator[](int i)
  {
    return data[i];
  }

  template <typename T, int W>
  template <typename OTHER_T>
  TSIMD_INLINE pack<OTHER_T, W> pack<T, W>::as()
  {
    pack<OTHER_T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = data[i];

    return result;
  }

  template <typename T, int W>
  TSIMD_INLINE T* pack<T, W>::begin()
  {
    return data;
  }

  template <typename T, int W>
  TSIMD_INLINE T* pack<T, W>::end()
  {
    return data + W;
  }

  template <typename T, int W>
  TSIMD_INLINE const T* pack<T, W>::cbegin() const
  {
    return data;
  }

  template <typename T, int W>
  TSIMD_INLINE const T* pack<T, W>::cend() const
  {
    return data + W;
  }

} // ::tsimd