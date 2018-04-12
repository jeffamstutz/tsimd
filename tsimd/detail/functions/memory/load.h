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

  // load() ///////////////////////////////////////////////////////////////////

  template <typename PACK_T>
  TSIMD_INLINE PACK_T load(const void *_src);

  template <typename PACK_T>
  TSIMD_INLINE PACK_T
  load(const void *_src,
       const mask<typename PACK_T::element_t, PACK_T::static_size> &m);

  // 1-wide //

  namespace detail {
    template <typename PACK_T, typename = traits::is_pack_of_width_t<PACK_T, 1>>
    TSIMD_INLINE PACK_T load1(const void *_src)
    {
      using T = const typename PACK_T::element_t;
      return PACK_T(*((T *)_src));
    }

    template <typename PACK_T, typename = traits::is_pack_of_width_t<PACK_T, 1>>
    TSIMD_INLINE PACK_T load1(const void *_src,
                              const mask_for_pack_t<PACK_T> &mask)
    {
      using T = const typename PACK_T::element_t;
      return PACK_T(mask[0] ? *((T *)_src) : T());
    }
  }  // namespace detail

  template <>
  TSIMD_INLINE vfloat1 load(const void *_src)
  {
    return detail::load1<vfloat1>(_src);
  }

  template <>
  TSIMD_INLINE vfloat1 load(const void *_src, const vboolf1 &m)
  {
    return detail::load1<vfloat1>(_src, m);
  }

  template <>
  TSIMD_INLINE vint1 load(const void *_src)
  {
    return detail::load1<vint1>(_src);
  }

  template <>
  TSIMD_INLINE vint1 load(const void *_src, const vboolf1 &m)
  {
    return detail::load1<vint1>(_src, m);
  }

  template <>
  TSIMD_INLINE vdouble1 load(const void *_src)
  {
    return detail::load1<vdouble1>(_src);
  }

  template <>
  TSIMD_INLINE vdouble1 load(const void *_src, const vboold1 &m)
  {
    return detail::load1<vdouble1>(_src, m);
  }

  template <>
  TSIMD_INLINE vllong1 load(const void *_src)
  {
    return detail::load1<vllong1>(_src);
  }

  template <>
  TSIMD_INLINE vllong1 load(const void *_src, const vboold1 &m)
  {
    return detail::load1<vllong1>(_src, m);
  }

  // 4-wide //

  template <>
  TSIMD_INLINE vfloat4 load(const void *_src)
  {
#if defined(__SSE4_2__)
    return _mm_load_ps((const float *)_src);
#else
    auto *src = (const typename vfloat4::element_t *)_src;
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = src[i];

    return result;
#endif
  }

  template <>
  TSIMD_INLINE vfloat4 load(const void *_src, const vboolf4 &mask)
  {
#if defined(__SSE4_2__)
    return _mm_and_ps(_mm_load_ps((const float *)_src), mask);
#else
    auto *src = (const typename vfloat4::element_t *)_src;
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      if (mask[i])
        result[i] = src[i];

    return result;
#endif
  }

  template <>
  TSIMD_INLINE vint4 load(const void *_src)
  {
#if defined(__SSE4_2__)
    return _mm_load_si128((const __m128i *)_src);
#else
    auto *src = (const typename vint4::element_t *)_src;
    vint4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = src[i];

    return result;
#endif
  }

  template <>
  TSIMD_INLINE vint4 load(const void *_src, const vboolf4 &mask)
  {
#if defined(__SSE4_2__)
    return _mm_and_si128(_mm_load_si128((const __m128i *)_src), mask);
#else
    auto *src = (const typename vint4::element_t *)_src;
    vint4 result;

    for (int i = 0; i < 4; ++i)
      if (mask[i])
        result[i] = src[i];

    return result;
#endif
  }

  template <>
  TSIMD_INLINE vdouble4 load(const void *_src)
  {
    auto *src = (const typename vdouble4::element_t *)_src;
    vdouble4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = src[i];

    return result;
  }

  template <>
  TSIMD_INLINE vdouble4 load(const void *_src, const vboold4 &mask)
  {
    auto *src = (const typename vdouble4::element_t *)_src;
    vdouble4 result;

    for (int i = 0; i < 4; ++i)
      if (mask[i])
        result[i] = src[i];

    return result;
  }

  template <>
  TSIMD_INLINE vllong4 load(const void *_src)
  {
    auto *src = (const typename vllong4::element_t *)_src;
    vllong4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = src[i];

    return result;
  }

  template <>
  TSIMD_INLINE vllong4 load(const void *_src, const vboold4 &mask)
  {
    auto *src = (const typename vllong4::element_t *)_src;
    vllong4 result;

    for (int i = 0; i < 4; ++i)
      if (mask[i])
        result[i] = src[i];

    return result;
  }

  // 8-wide //

  template <>
  TSIMD_INLINE vfloat8 load(const void *_src)
  {
#if defined(__AVX512__) || defined(__AVX__)
    return _mm256_load_ps((const float *)_src);
#else
    auto *src = (const typename vfloat8::element_t *)_src;
    return vfloat8(load<vfloat4>(src), load<vfloat4>(src + 4));
#endif
  }

  template <>
  TSIMD_INLINE vfloat8 load(const void *_src, const vboolf8 &mask)
  {
#if defined(__AVX512VL__)
    return _mm256_mask_load_ps(_mm256_setzero_ps(), mask, (float*)_src);
#elif defined(__AVX__)
    return _mm256_maskload_ps((const float *)_src, _mm256_castps_si256(mask));
#else
    auto *src = (const typename vfloat16::element_t *)_src;
    return vfloat8(load<vfloat4>(src, vboolf4(mask.vl)),
                   load<vfloat4>(src + 4, vboolf4(mask.vh)));
#endif
  }

  template <>
  TSIMD_INLINE vint8 load(const void *_src)
  {
#if defined(__AVX2__) || defined(__AVX__)
    return _mm256_castps_si256(_mm256_load_ps((const float *)_src));
#else
    auto *src = (const typename vint16::element_t *)_src;
    return vint8(load<vint4>(src), load<vint4>(src + 4));
#endif
  }

  template <>
  TSIMD_INLINE vint8 load(const void *_src, const vboolf8 &mask)
  {
#if defined(__AVX512VL__)
    return _mm256_mask_load_epi32 (_mm256_setzero_si256(), mask, _src);
#elif defined(__AVX__)
    return _mm256_castps_si256(
        _mm256_maskload_ps((const float *)_src, _mm256_castps_si256(mask)));
#else
    auto *src = (const typename vint16::element_t *)_src;
    return vint8(load<vint4>(src, vboolf4(mask.vl)),
                 load<vint4>(src + 4, vboolf4(mask.vh)));
#endif
  }

  template <>
  TSIMD_INLINE vdouble8 load(const void *_src)
  {
#if defined(__AVX512F__)
    return _mm512_load_pd(_src);
#else
    auto *src = (const typename vdouble8::element_t *)_src;
    return vdouble8(load<vdouble4>(src), load<vdouble4>(src + 4));
#endif
  }

  template <>
  TSIMD_INLINE vdouble8 load(const void *_src, const vboold8 &mask)
  {
#if defined(__AVX512F__)
    return _mm512_mask_load_pd(_mm512_setzero_pd(), mask, _src);
#else
    auto *src = (const typename vdouble16::element_t *)_src;
    return vdouble8(load<vdouble4>(src, vboold4(mask.vl)),
                    load<vdouble4>(src + 4, vboold4(mask.vh)));
#endif
  }

  template <>
  TSIMD_INLINE vllong8 load(const void *_src)
  {
#if defined(__AVX512F__)
    return _mm512_load_epi64(_src);
#else
    auto *src = (const typename vllong16::element_t *)_src;
    return vllong8(load<vllong4>(src), load<vllong4>(src + 4));
#endif
  }

  template <>
  TSIMD_INLINE vllong8 load(const void *_src, const vboold8 &mask)
  {
#if defined(__AVX512F__)
    return _mm512_mask_load_epi64(_mm512_setzero_si512(), mask, _src);
#else
    auto *src = (const typename vllong16::element_t *)_src;
    return vllong8(load<vllong4>(src, vboold4(mask.vl)),
                  load<vllong4>(src + 4, vboold4(mask.vh)));
#endif
  }

  // 16-wide //

  template <>
  TSIMD_INLINE vfloat16 load(const void *_src)
  {
#if defined(__AVX512F__)
    return _mm512_load_ps((const float *)_src);
#else
    auto *src = (const typename vfloat16::element_t *)_src;
    return vfloat16(load<vfloat8>(src), load<vfloat8>(src + 8));
#endif
  }

  template <>
  TSIMD_INLINE vfloat16 load(const void *_src, const vboolf16 &mask)
  {
#if defined(__AVX512F__)
    return _mm512_mask_load_ps(_mm512_setzero_ps(), mask, (float *)_src);
#else
    auto *src = (const typename vfloat16::element_t *)_src;
    return vfloat16(load<vfloat8>(src, vboolf8(mask.vl)),
                    load<vfloat8>(src + 8, vboolf8(mask.vh)));
#endif
  }

  template <>
  TSIMD_INLINE vint16 load(const void *_src)
  {
#if defined(__AVX512F__)
    return _mm512_load_si512((const int *)_src);
#else
    auto *src = (const typename vint16::element_t *)_src;
    return vint16(load<vint8>(src), load<vint8>(src + 8));
#endif
  }

  template <>
  TSIMD_INLINE vint16 load(const void *_src, const vboolf16 &mask)
  {
#if defined(__AVX512F__)
    return _mm512_mask_load_epi32(_mm512_setzero_epi32(), mask, _src);
#else
    auto *src = (const typename vint16::element_t *)_src;
    return vint16(load<vint8>(src, vboolf8(mask.vl)),
                  load<vint8>(src + 8, vboolf8(mask.vh)));
#endif
  }

  template <>
  TSIMD_INLINE vdouble16 load(const void *_src)
  {
    auto *src = (const typename vdouble16::element_t *)_src;
    return vdouble16(load<vdouble8>(src), load<vdouble8>(src + 8));
  }

  template <>
  TSIMD_INLINE vdouble16 load(const void *_src, const vboold16 &mask)
  {
    auto *src = (const typename vdouble16::element_t *)_src;
    return vdouble16(load<vdouble8>(src, vboold8(mask.vl)),
                     load<vdouble8>(src + 8, vboold8(mask.vh)));
  }

  template <>
  TSIMD_INLINE vllong16 load(const void *_src)
  {
    auto *src = (const typename vllong16::element_t *)_src;
    return vllong16(load<vllong8>(src), load<vllong8>(src + 8));
  }

  template <>
  TSIMD_INLINE vllong16 load(const void *_src, const vboold16 &mask)
  {
    auto *src = (const typename vllong16::element_t *)_src;
    return vllong16(load<vllong8>(src, vboold8(mask.vl)),
                    load<vllong8>(src + 8, vboold8(mask.vh)));
  }

}  // namespace tsimd
