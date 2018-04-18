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

#include "../../pack.h"

namespace tsimd {

  // 1-wide //

  template <typename T>
  TSIMD_INLINE mask<T, 1> operator>(const pack<T, 1> &p1, const pack<T, 1> &p2)
  {
    return mask<T, 1>(p1[0] > p2[0]);
  }

  // 4-wide //

  TSIMD_INLINE vboolf4 operator>(const vfloat4 &p1, const vfloat4 &p2)
  {
#if defined(__SSE4_2__)
    return _mm_cmpgt_ps(p1, p2);
#else
    vboolf4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = (p1[i] > p2[i]);

    return result;
#endif
  }

  TSIMD_INLINE vboolf4 operator>(const vint4 &p1, const vint4 &p2)
  {
#if defined(__SSE4_2__)
    return _mm_castsi128_ps(_mm_cmpgt_epi32(p1, p2));
#else
    vboolf4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = (p1[i] > p2[i]);

    return result;
#endif
  }

  TSIMD_INLINE vboold4 operator>(const vdouble4 &p1, const vdouble4 &p2)
  {
    vboold4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = (p1[i] > p2[i]);

    return result;
  }

  TSIMD_INLINE vboold4 operator>(const vllong4 &p1, const vllong4 &p2)
  {
    vboold4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = (p1[i] > p2[i]);

    return result;
  }

  // 8-wide //

  TSIMD_INLINE vboolf8 operator>(const vfloat8 &p1, const vfloat8 &p2)
  {
#if defined(__AVX512VL__)
    return _mm256_cmp_ps_mask(p1, p2, _MM_CMPINT_GT);
#elif defined(__AVX2__) || defined(__AVX__)
    return _mm256_cmp_ps(p1, p2, _CMP_GT_OQ);
#else
    return vboolf8(vfloat4(p1.vl) > vfloat4(p2.vl),
                   vfloat4(p1.vh) > vfloat4(p2.vh));
#endif
  }

  TSIMD_INLINE vboolf8 operator>(const vint8 &p1, const vint8 &p2)
  {
#if defined(__AVX512VL__)
    return _mm256_cmp_epi32_mask(p1, p2, _MM_CMPINT_GT);
#elif defined(__AVX2__)
    return _mm256_castsi256_ps(_mm256_cmpgt_epi32(p1, p2));
#elif defined(__AVX__)
    return vboolf8(_mm_castsi128_ps(_mm_cmpgt_epi32(p1.vl, p2.vl)),
                   _mm_castsi128_ps(_mm_cmpgt_epi32(p1.vh, p2.vh)));
#else
    return vboolf8(vint4(p1.vl) > vint4(p2.vl), vint4(p1.vh) > vint4(p2.vh));
#endif
  }

  TSIMD_INLINE vboold8 operator>(const vdouble8 &p1, const vdouble8 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_cmp_pd_mask(p1, p2, _MM_CMPINT_GT);
#else
    return vboold8(vdouble4(p1.vl) > vdouble4(p2.vl),
                   vdouble4(p1.vh) > vdouble4(p2.vh));
#endif
  }

  TSIMD_INLINE vboold8 operator>(const vllong8 &p1, const vllong8 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_cmp_epi64_mask(p1, p2, _MM_CMPINT_GT);
#else
    return vboold8(vllong4(p1.vl) > vllong4(p2.vl),
                   vllong4(p1.vh) > vllong4(p2.vh));
#endif
  }

  // 16-wide //

  TSIMD_INLINE vboolf16 operator>(const vfloat16 &p1, const vfloat16 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_cmp_ps_mask(p1, p2, _MM_CMPINT_GT);
#else
    return vboolf16(vfloat8(p1.vl) > vfloat8(p2.vl),
                    vfloat8(p1.vh) > vfloat8(p2.vh));
#endif
  }

  TSIMD_INLINE vboolf16 operator>(const vint16 &p1, const vint16 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_cmp_epi32_mask(p1, p2, _MM_CMPINT_GT);
#else
    return vboolf16(vint8(p1.vl) > vint8(p2.vl), vint8(p1.vh) > vint8(p2.vh));
#endif
  }

  TSIMD_INLINE vboold16 operator>(const vdouble16 &p1, const vdouble16 &p2)
  {
    return vboold16(vdouble8(p1.vl) > vdouble8(p2.vl),
                    vdouble8(p1.vh) > vdouble8(p2.vh));
  }

  TSIMD_INLINE vboold16 operator>(const vllong16 &p1, const vllong16 &p2)
  {
    return vboold16(vllong8(p1.vl) > vllong8(p2.vl),
                    vllong8(p1.vh) > vllong8(p2.vh));
  }

  // Inferred pack-scalar operators ///////////////////////////////////////////

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::valid_pack_scalar_operator_t<T, OTHER_T>>
  TSIMD_INLINE mask<T, W> operator>(const pack<T, W> &p1, const OTHER_T &v)
  {
    return p1 > pack<T, W>(v);
  }

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::valid_pack_scalar_operator_t<T, OTHER_T>>
  TSIMD_INLINE mask<T, W> operator>(const OTHER_T &v, const pack<T, W> &p1)
  {
    return pack<T, W>(v) > p1;
  }

}  // namespace tsimd
