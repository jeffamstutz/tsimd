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

  // binary operator&() ///////////////////////////////////////////////////////

  // 1-wide //

  template <typename T>
  TSIMD_INLINE pack<T, 1> operator|(const pack<T, 1> &p1, const pack<T, 1> &p2)
  {
    return pack<T, 1>(p1[0] | p2[0]);
  }

  // 4-wide //

  // TODO

  // 8-wide //

  TSIMD_INLINE vfloat8 operator|(const vfloat8 &p1, const vfloat8 &p2)
  {
#if defined(__AVX512__) || defined(__AVX__)
    return _mm256_or_ps(p1, p2);
#else
    vfloat8 result;

    for (int i = 0; i < 8; ++i)
      result[i] = p1[i] | p2[i];

    return result;
#endif
  }

  TSIMD_INLINE vint8 operator|(const vint8 &p1, const vint8 &p2)
  {
#if defined(__AVX512__) || defined(__AVX2__)
    return _mm256_or_si256(p1, p2);
#elif defined(__AVX__)
    return _mm256_castps_si256(
        _mm256_or_ps(_mm256_castsi256_ps(p1), _mm256_castsi256_ps(p2)));
#else
    vint8 result;

    for (int i = 0; i < 8; ++i)
      result[i] = p1[i] | p2[i];

    return result;
#endif
  }

  TSIMD_INLINE vbool8 operator|(const vbool8 &p1, const vbool8 &p2)
  {
#if defined(__AVX512__) || defined(__AVX__)
    return _mm256_or_ps(p1, p2);
#else
    vbool8 result;

    for (int i = 0; i < 8; ++i)
      result[i] = p1[i] | p2[i];

    return result;
#endif
  }

  // 16-wide //

  // TODO

  template <
      typename T,
      int W,
      typename OTHER_T,
      typename = traits::can_convert<OTHER_T, T>
  >
  TSIMD_INLINE pack<T, W> operator|(const pack<T, W> &p1, const OTHER_T &v)
  {
    return p1 | pack<T, W>(v);
  }

  template <
      typename T,
      int W,
      typename OTHER_T,
      typename = traits::can_convert<OTHER_T, T>
  >
  TSIMD_INLINE pack<T, W> operator|(const OTHER_T &v, const pack<T, W> &p1)
  {
    return pack<T, W>(v) | p1;
  }

}  // namespace tsimd