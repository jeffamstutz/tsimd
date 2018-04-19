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
  TSIMD_INLINE pack<T, 1> ceil(const pack<T, 1> &p)
  {
    return pack<T, 1>(std::ceil(p[0]));
  }

  // 4-wide //

  TSIMD_INLINE vfloat4 ceil(const vfloat4 &p)
  {
#if defined(__SSE4_2__)
    return _mm_round_ps(p, _MM_FROUND_TO_POS_INF);
#else
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = std::ceil(p[i]);

    return result;
#endif
  }

  TSIMD_INLINE vdouble4 ceil(const vdouble4 &p)
  {
#if defined(__AVX__)
    return _mm256_round_pd(p, _MM_FROUND_TO_POS_INF);
#else
    vdouble4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = std::ceil(p[i]);

    return result;
#endif
  }

  // 8-wide //

  TSIMD_INLINE vfloat8 ceil(const vfloat8 &p)
  {
#if defined(__AVX__)
    return _mm256_round_ps(p, _MM_FROUND_TO_POS_INF);
#else
    return vfloat8(ceil(vfloat4(p.vl)), ceil(vfloat4(p.vh)));
#endif
  }

  TSIMD_INLINE vdouble8 ceil(const vdouble8 &p)
  {
#if defined(__AVX512F__)
    return _mm512_ceil_pd(p);
#else
    return vdouble8(ceil(vdouble4(p.vl)), ceil(vdouble4(p.vh)));
#endif
  }

  // 16-wide //

  TSIMD_INLINE vfloat16 ceil(const vfloat16 &p)
  {
#if defined(__AVX512F__)
    return _mm512_ceil_ps(p);
#else
    return vfloat16(ceil(vfloat8(p.vl)), ceil(vfloat8(p.vh)));
#endif
  }

  TSIMD_INLINE vdouble16 ceil(const vdouble16 &p)
  {
    return vdouble16(ceil(vdouble8(p.vl)), ceil(vdouble8(p.vh)));
  }

}  // namespace tsimd
