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

#include <cmath>

#include "../../pack.h"

namespace tsimd {

  // 1-wide //

  template <typename T>
  TSIMD_INLINE pack<T, 1> abs(const pack<T, 1> &p)
  {
    return pack<T, 1>(std::abs(p[0]));
  }

  // 4-wide //

  TSIMD_INLINE vfloat4 abs(const vfloat4 &p)
  {
#if defined(__SSE__)
    return _mm_and_ps(p, _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff)));
#else
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = std::abs(p[i]);

    return result;
#endif
  }

  TSIMD_INLINE vint4 abs(const vint4 &p)
  {
#if defined(__SSE__)
    return _mm_abs_epi32(p);
#else
    vint4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = std::abs(p[i]);

    return result;
#endif
  }

  // 8-wide //

  TSIMD_INLINE vfloat8 abs(const vfloat8 &p)
  {
#if defined(__AVX2__) || defined(__AVX__)
    return _mm256_and_ps(p, _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff)));
#else
    return vfloat8(abs(vfloat4(p.vl)), abs(vfloat4(p.vh)));
#endif
  }

  TSIMD_INLINE vint8 abs(const vint8 &p)
  {
#if defined(__AVX2__)
    return _mm256_abs_epi32(p);
#elif defined(__AVX__)
    return vint8(_mm_abs_epi32(p.vl), _mm_abs_epi32(p.vh));
#else
    return vint8(abs(vint4(p.vl)), abs(vint4(p.vh)));
#endif
  }

  // 16-wide //

  TSIMD_INLINE vfloat16 abs(const vfloat16 &p)
  {
#if defined(__AVX512F__)
    return _mm512_castsi512_ps(
      _mm512_and_epi32(
        _mm512_castps_si512(p),_mm512_set1_epi32(0x7FFFFFFF)));
#else
    return vfloat16(abs(vfloat8(p.vl)), abs(vfloat8(p.vh)));
#endif
  }

  TSIMD_INLINE vint16 abs(const vint16 &p)
  {
#if defined(__AVX512F__)
    return _mm512_abs_epi32(p);
#else
    return vint16(abs(vint8(p.vl)), abs(vint8(p.vh)));
#endif
  }

}  // namespace tsimd