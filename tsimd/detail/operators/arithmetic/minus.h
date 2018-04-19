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
  TSIMD_INLINE pack<T, 1> operator-(const pack<T, 1> &p1, const pack<T, 1> &p2)
  {
    return pack<T, 1>(p1[0] - p2[0]);
  }

  // 4-wide //

  TSIMD_INLINE vfloat4 operator-(const vfloat4 &p1, const vfloat4 &p2)
  {
#if defined(__SSE4_2__)
    return _mm_sub_ps(p1, p2);
#else
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = (p1[i] - p2[i]);

    return result;
#endif
  }

  TSIMD_INLINE vint4 operator-(const vint4 &p1, const vint4 &p2)
  {
#if defined(__SSE4_2__)
    return _mm_sub_epi32(p1, p2);
#else
    vint4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = (p1[i] - p2[i]);

    return result;
#endif
  }

  TSIMD_INLINE vdouble4 operator-(const vdouble4 &p1, const vdouble4 &p2)
  {
#if defined(__AVX__)
    return _mm256_sub_pd(p1, p2);
#else
    vdouble4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = (p1[i] - p2[i]);

    return result;
#endif
  }

  TSIMD_INLINE vllong4 operator-(const vllong4 &p1, const vllong4 &p2)
  {
#if defined(__AVX2__)
    return _mm256_sub_epi64(p1, p2);
#else
    vllong4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = (p1[i] - p2[i]);

    return result;
#endif
  }

  // 8-wide //

  TSIMD_INLINE vfloat8 operator-(const vfloat8 &p1, const vfloat8 &p2)
  {
#if defined(__AVX__)
    return _mm256_sub_ps(p1, p2);
#else
    return vfloat8(vfloat4(p1.vl) - vfloat4(p2.vl),
                   vfloat4(p1.vh) - vfloat4(p2.vh));
#endif
  }

  TSIMD_INLINE vint8 operator-(const vint8 &p1, const vint8 &p2)
  {
#if defined(__AVX2__)
    return _mm256_sub_epi32(p1, p2);
#elif defined(__AVX__)
    return vint8(_mm_sub_epi32(p1.vl, p2.vl), _mm_sub_epi32(p1.vh, p2.vh));
#else
    return vint8(vint4(p1.vl) - vint4(p2.vl), vint4(p1.vh) - vint4(p2.vh));
#endif
  }

  TSIMD_INLINE vdouble8 operator-(const vdouble8 &p1, const vdouble8 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_sub_pd(p1, p2);
#else
    return vdouble8(vdouble4(p1.vl) - vdouble4(p2.vl),
                    vdouble4(p1.vh) - vdouble4(p2.vh));
#endif
  }

  TSIMD_INLINE vllong8 operator-(const vllong8 &p1, const vllong8 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_sub_epi64(p1, p2);
#else
    return vllong8(vllong4(p1.vl) - vllong4(p2.vl),
                   vllong4(p1.vh) - vllong4(p2.vh));
#endif
  }

  // 16-wide //

  TSIMD_INLINE vfloat16 operator-(const vfloat16 &p1, const vfloat16 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_sub_ps(p1, p2);
#else
    return vfloat16(vfloat8(p1.vl) - vfloat8(p2.vl),
                    vfloat8(p1.vh) - vfloat8(p2.vh));
#endif
  }

  TSIMD_INLINE vint16 operator-(const vint16 &p1, const vint16 &p2)
  {
#if defined(__AVX512F__)
    return _mm512_sub_epi32(p1, p2);
#else
    return vint16(vint8(p1.vl) - vint8(p2.vl), vint8(p1.vh) - vint8(p2.vh));
#endif
  }

  TSIMD_INLINE vdouble16 operator-(const vdouble16 &p1, const vdouble16 &p2)
  {
    return vdouble16(vdouble8(p1.vl) - vdouble8(p2.vl),
                     vdouble8(p1.vh) - vdouble8(p2.vh));
  }

  TSIMD_INLINE vllong16 operator-(const vllong16 &p1, const vllong16 &p2)
  {
    return vllong16(vllong8(p1.vl) - vllong8(p2.vl),
                    vllong8(p1.vh) - vllong8(p2.vh));
  }

  // Inferred pack-pack promotion operators (e.g. 'vint' to 'vfloat') /////////

  template <typename T1,
            typename T2,
            int W,
            typename = traits::is_not_same_t<T1, T2>>
  TSIMD_INLINE auto operator-(const pack<T1, W> &p1, const pack<T2, W> &p2)
      -> pack<decltype(T1() - T2()), W>
  {
    using result_pack = pack<decltype(T1() - T2()), W>;
    return result_pack(p1) - result_pack(p2);
  }

  // Inferred pack-scalar operators ///////////////////////////////////////////

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::valid_pack_scalar_operator_t<T, OTHER_T>>
  TSIMD_INLINE auto operator-(const pack<T, W> &p1, const OTHER_T &v)
      -> pack<decltype(T() - OTHER_T()), W>
  {
    using result_pack = pack<decltype(T() - OTHER_T()), W>;
    return result_pack(p1) - result_pack(v);
  }

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::valid_pack_scalar_operator_t<T, OTHER_T>>
  TSIMD_INLINE auto operator-(const OTHER_T &v, const pack<T, W> &p1)
      -> pack<decltype(OTHER_T() - T()), W>
  {
    using result_pack = pack<decltype(OTHER_T() - T()), W>;
    return result_pack(v) - result_pack(p1);
  }

  // Inferred binary operator-=() /////////////////////////////////////////////

  template <typename T1, typename T2, int W>
  TSIMD_INLINE pack<T1, W> &operator-=(pack<T1, W> &p1, const pack<T2, W> &p2)
  {
    return p1 = (p1 - p2);
  }

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::valid_pack_scalar_operator_t<T, OTHER_T>>
  TSIMD_INLINE pack<T, W> &operator-=(pack<T, W> &p1, const OTHER_T &v)
  {
    return p1 = (p1 - pack<T, W>(v));
  }

}  // namespace tsimd
