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

  // binary operator|() ///////////////////////////////////////////////////////

  // 1-wide //

  template <typename T, typename = traits::is_not_floating_point_t<T>>
  TSIMD_INLINE pack<T, 1> operator|(const pack<T, 1> &p1, const pack<T, 1> &p2)
  {
    return pack<T, 1>(p1[0] | p2[0]);
  }

  // 4-wide //

  TSIMD_INLINE vint4 operator|(const vint4 &p1, const vint4 &p2)
  {
#if defined(__SSE4_2__)
    return _mm_or_si128(p1, p2);
#else
    vint4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = p1[i] | p2[i];

    return result;
#endif
  }

  TSIMD_INLINE vboolf4 operator|(const vboolf4 &p1, const vboolf4 &p2)
  {
#if defined(__SSE4_2__)
    return _mm_or_ps(p1, p2);
#else
    vboolf4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = p1[i] | p2[i];

    return result;
#endif
  }

  TSIMD_INLINE vllong4 operator|(const vllong4 &p1, const vllong4 &p2)
  {
#if defined(__AVX2__)
    return _mm256_or_si256(p1, p2);
#else
    vllong4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = p1[i] | p2[i];

    return result;
#endif
  }

  TSIMD_INLINE vboold4 operator|(const vboold4 &p1, const vboold4 &p2)
  {
#if defined(__AVX__)
    return _mm256_or_pd(p1, p2);
#else
    vboold4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = p1[i] | p2[i];

    return result;
#endif
  }

  // 8-wide //

  TSIMD_INLINE vint8 operator|(const vint8 &p1, const vint8 &p2)
  {
#if defined(__AVX2__)
    return _mm256_or_si256(p1, p2);
#elif defined(__AVX__)
    return _mm256_castps_si256(
        _mm256_or_ps(_mm256_castsi256_ps(p1), _mm256_castsi256_ps(p2)));
#else
    return vint8(vint4(p1.vl) | vint4(p2.vl), vint4(p1.vh) | vint4(p2.vh));
#endif
  }

  TSIMD_INLINE vboolf8 operator|(const vboolf8 &p1, const vboolf8 &p2)
  {
#if defined(__AVX512VL__)
    return _mm512_kor(p1, p2);
#elif defined(__AVX__)
    return _mm256_or_ps(p1, p2);
#else
    return vboolf8(vboolf4(p1.vl) | vboolf4(p2.vl),
                   vboolf4(p1.vh) | vboolf4(p2.vh));
#endif
  }

  TSIMD_INLINE vllong8 operator|(const vllong8 &p1, const vllong8 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_or_epi64(p1, p2);
#else
    return vllong8(vllong4(p1.vl) | vllong4(p2.vl),
                   vllong4(p1.vh) | vllong4(p2.vh));
#endif
  }

  TSIMD_INLINE vboold8 operator|(const vboold8 &p1, const vboold8 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_kor(p1, p2);
#else
    return vboold8(vboold4(p1.vl) | vboold4(p2.vl),
                   vboold4(p1.vh) | vboold4(p2.vh));
#endif
  }

  // 16-wide //

  TSIMD_INLINE vint16 operator|(const vint16 &p1, const vint16 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_or_epi32(p1, p2);
#else
    return vint16(vint8(p1.vl) | vint8(p2.vl), vint8(p1.vh) | vint8(p2.vh));
#endif
  }

  TSIMD_INLINE vboolf16 operator|(const vboolf16 &p1, const vboolf16 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_kor(p1, p2);
#else
    return vboolf16(vboolf8(p1.vl) | vboolf8(p2.vl),
                    vboolf8(p1.vh) | vboolf8(p2.vh));
#endif
  }

  TSIMD_INLINE vllong16 operator|(const vllong16 &p1, const vllong16 &p2)
  {
    return vllong16(vllong8(p1.vl) | vllong8(p2.vl),
                    vllong8(p1.vh) | vllong8(p2.vh));
  }

  TSIMD_INLINE vboold16 operator|(const vboold16 &p1, const vboold16 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_kor(p1, p2);
#else
    return vboold16(vboold8(p1.vl) | vboold8(p2.vl),
                    vboold8(p1.vh) | vboold8(p2.vh));
#endif
  }

  // Inferred pack-scalar operators ///////////////////////////////////////////

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::valid_pack_scalar_operator_t<T, OTHER_T>>
  TSIMD_INLINE pack<T, W> operator|(const pack<T, W> &p1, const OTHER_T &v)
  {
    return p1 | pack<T, W>(v);
  }

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::valid_pack_scalar_operator_t<T, OTHER_T>>
  TSIMD_INLINE pack<T, W> operator|(const OTHER_T &v, const pack<T, W> &p1)
  {
    return pack<T, W>(v) | p1;
  }

  // Inferred operator|=() ////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> operator|=(pack<T, W> &p1, const pack<T, W> &p2)
  {
    return p1 = p1 | p2;
  }

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::valid_pack_scalar_operator_t<T, OTHER_T>>
  TSIMD_INLINE pack<T, W> operator|=(pack<T, W> &p1, const OTHER_T &v)
  {
    return p1 = p1 | pack<T, W>(v);
  }

}  // namespace tsimd
