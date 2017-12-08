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

#include "../../pack.h"

namespace tsimd {

  // 1-wide //

  template <typename T>
  TSIMD_INLINE pack<T, 1> operator*(const pack<T, 1> &p1, const pack<T, 1> &p2)
  {
    return pack<T, 1>(p1[0] * p2[0]);
  }

  // 4-wide //

  TSIMD_INLINE vfloat4 operator*(const vfloat4 &p1, const vfloat4 &p2)
  {
#if defined(__SSE__)
    return _mm_mul_ps(p1, p2);
#else
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = (p1[i] * p2[i]);

    return result;
#endif
  }

  TSIMD_INLINE vint4 operator*(const vint4 &p1, const vint4 &p2)
  {
#if defined(__SSE4_1__)
    return _mm_mullo_epi32(p1, p2);
#else
    vint4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = (p1[i] * p2[i]);

    return result;
#endif
  }

  // 8-wide //

  TSIMD_INLINE vfloat8 operator*(const vfloat8 &p1, const vfloat8 &p2)
  {
#if defined(__AVX512__) || defined(__AVX2__) || defined(__AVX__)
    return _mm256_mul_ps(p1, p2);
#else
    return vfloat8(vfloat4(p1.vl) * vfloat4(p2.vl),
                   vfloat4(p1.vh) * vfloat4(p2.vh));
#endif
  }

  TSIMD_INLINE vint8 operator*(const vint8 &p1, const vint8 &p2)
  {
#if defined(__AVX512__) || defined(__AVX2__)
    return _mm256_mullo_epi32(p1, p2);
#elif defined(__AVX__)
    return vint8(_mm_mullo_epi32(p1.vl, p2.vl), _mm_mullo_epi32(p1.vh, p2.vh));
#else
    return vint8(vint4(p1.vl) * vint4(p2.vl), vint4(p1.vh) * vint4(p2.vh));
#endif
  }

  // 16-wide //

  TSIMD_INLINE vfloat16 operator*(const vfloat16 &p1, const vfloat16 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_mul_ps(p1, p2);
#else
    return vfloat16(vfloat8(p1.vl) * vfloat8(p2.vl),
                    vfloat8(p1.vh) * vfloat8(p2.vh));
#endif
  }

  TSIMD_INLINE vint16 operator*(const vint16 &p1, const vint16 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_mullo_epi32(p1, p2);
#else
    return vint16(vint8(p1.vl) * vint8(p2.vl), vint8(p1.vh) * vint8(p2.vh));
#endif
  }

  // Inferred pack-scalar operators ///////////////////////////////////////////

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::can_convert<OTHER_T, T>>
  TSIMD_INLINE pack<T, W> operator*(const pack<T, W> &p1, const OTHER_T &v)
  {
    return p1 * pack<T, W>(v);
  }

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::can_convert<OTHER_T, T>>
  TSIMD_INLINE pack<T, W> operator*(const OTHER_T &v, const pack<T, W> &p1)
  {
    return pack<T, W>(v) * p1;
  }

  // Inferred binary operator*=() /////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> &operator*=(pack<T, W> &p1, const pack<T, W> &p2)
  {
    return p1 = (p1 * p2);
  }

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::can_convert<OTHER_T, T>>
  TSIMD_INLINE pack<T, W> &operator*=(pack<T, W> &p1, const OTHER_T &v)
  {
    return p1 = (p1 * pack<T, W>(v));
  }

}  // namespace tsimd
