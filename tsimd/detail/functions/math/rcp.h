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
  TSIMD_INLINE pack<T, 1> rcp(const pack<T, 1> &p)
  {
    return pack<T, 1>(T(1) / p[0]);
  }

  // 4-wide //

  TSIMD_INLINE vfloat4 rcp(const vfloat4 &p)
  {
#if defined(__SSE4_2__)
    return _mm_rcp_ps(p);
#else
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = 1.f / p[i];

    return result;
#endif
  }

  TSIMD_INLINE vdouble4 rcp(const vdouble4 &p)
  {
    vdouble4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = 1. / p[i];

    return result;
  }

  // 8-wide //

  TSIMD_INLINE vfloat8 rcp(const vfloat8 &p)
  {
#if defined(__AVX512VL__)
    const vfloat8 r = _mm256_rcp14_ps(p);
#elif defined(__AVX__)
    const vfloat8 r = _mm256_rcp_ps(p);
#endif

#if defined(__AVX2__)
    return _mm256_mul_ps(r, _mm256_fnmadd_ps(r, p, vfloat8(2.0f)));
#elif defined(__AVX__)
    return _mm256_mul_ps(r, _mm256_sub_ps(vfloat8(2.0f), _mm256_mul_ps(r, p)));
#else
    return vfloat8(rcp(vfloat4(p.vl)), rcp(vfloat4(p.vh)));
#endif
  }

  TSIMD_INLINE vdouble8 rcp(const vdouble8 &p)
  {
#if defined(__AVX512F__)
    return _mm512_rcp14_pd(p);
#else
    return vdouble8(rcp(vdouble4(p.vl)), rcp(vdouble4(p.vh)));
#endif
  }

  // 16-wide //

  TSIMD_INLINE vfloat16 rcp(const vfloat16 &p)
  {
#if defined(__AVX512F__)
    return _mm512_rcp14_ps(p);
#else
    return vfloat16(rcp(vfloat8(p.vl)), rcp(vfloat8(p.vh)));
#endif
  }

  TSIMD_INLINE vdouble16 rcp(const vdouble16 &p)
  {
    return vdouble16(rcp(vdouble8(p.vl)), rcp(vdouble8(p.vh)));
  }

}  // namespace tsimd
