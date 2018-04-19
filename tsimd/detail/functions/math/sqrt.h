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
  TSIMD_INLINE pack<T, 1> sqrt(const pack<T, 1> &p)
  {
    return pack<T, 1>(std::sqrt(p[0]));
  }

  // 4-wide //

  TSIMD_INLINE vfloat4 sqrt(const vfloat4 &p)
  {
#if defined(__SSE4_2__)
    return _mm_sqrt_ps(p);
#else
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = std::sqrt(p[i]);

    return result;
#endif
  }

  TSIMD_INLINE vdouble4 sqrt(const vdouble4 &p)
  {
#if defined(__AVX__)
    return _mm256_sqrt_pd(p);
#else
    vdouble4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = std::sqrt(p[i]);

    return result;
#endif
  }

  // 8-wide //

  TSIMD_INLINE vfloat8 sqrt(const vfloat8 &p)
  {
#if defined(__AVX__)
    return _mm256_sqrt_ps(p);
#else
    return vfloat8(sqrt(vfloat4(p.vl)), sqrt(vfloat4(p.vh)));
#endif
  }

  TSIMD_INLINE vdouble8 sqrt(const vdouble8 &p)
  {
#if defined(__AVX512F__)
    return _mm512_sqrt_pd(p);
#else
    return vdouble8(sqrt(vdouble4(p.vl)), sqrt(vdouble4(p.vh)));
#endif
  }

  // 16-wide //

  TSIMD_INLINE vfloat16 sqrt(const vfloat16 &p)
  {
#if defined(__AVX512F__)
    return _mm512_sqrt_ps(p);
#else
    return vfloat16(sqrt(vfloat8(p.vl)), sqrt(vfloat8(p.vh)));
#endif
  }

  TSIMD_INLINE vdouble16 sqrt(const vdouble16 &p)
  {
    return vdouble16(sqrt(vdouble8(p.vl)), sqrt(vdouble8(p.vh)));
  }

}  // namespace tsimd
