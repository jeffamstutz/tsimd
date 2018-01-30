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
#include "load.h"

namespace tsimd {

  // store() //////////////////////////////////////////////////////////////////

  template <typename PACK_T>
  TSIMD_INLINE void store(const PACK_T &p, void *_dst);

  template <typename PACK_T>
  TSIMD_INLINE void store(
      const PACK_T &p,
      void *_dst,
      const mask<typename PACK_T::element_t, PACK_T::static_size> &m);

  // 1-wide //

  template <typename T>
  TSIMD_INLINE void store(const pack<T, 1> &v, void *_dst)
  {
    *((T *)_dst) = v[0];
  }

  template <typename T>
  TSIMD_INLINE void store(const pack<T, 1> &v, void *_dst, const vboolf1 &mask)
  {
    if (mask[0])
      *((T *)_dst) = v[0];
  }

  // 4-wide //

  template <>
  TSIMD_INLINE void store(const vfloat4 &v, void *_dst)
  {
#if defined(__SSE4_2__)
    _mm_store_ps((float *)_dst, v);
#else
    auto *dst = (typename vfloat4::element_t *)_dst;

    for (int i = 0; i < 4; ++i)
      dst[i] = v[i];
#endif
  }

  template <>
  TSIMD_INLINE void store(const vfloat4 &v, void *_dst, const vboolf4 &mask)
  {
#if defined(__SSE4_2__)
    store(select(mask, v, load<vfloat4>(_dst)), _dst);
#else
    auto *dst = (typename vfloat4::element_t *)_dst;

    for (int i = 0; i < 4; ++i)
      if (mask[i])
        dst[i] = v[i];
#endif
  }

  template <>
  TSIMD_INLINE void store(const vint4 &v, void *_dst)
  {
#if defined(__SSE4_2__)
    _mm_store_si128((__m128i *)_dst, v);
#else
    auto *dst = (typename vint4::element_t *)_dst;

    for (int i = 0; i < 4; ++i)
      dst[i] = v[i];
#endif
  }

  template <>
  TSIMD_INLINE void store(const vint4 &v, void *_dst, const vboolf4 &mask)
  {
#if defined(__SSE4_2__)
    store(select(mask, v, load<vint4>(_dst)), _dst);
#else
    auto *dst = (typename vint4::element_t *)_dst;

    for (int i = 0; i < 4; ++i)
      if (mask[i])
        dst[i] = v[i];
#endif
  }

  template <>
  TSIMD_INLINE void store(const vdouble4 &v, void *_dst)
  {
    auto *dst = (typename vdouble4::element_t *)_dst;

    for (int i = 0; i < 4; ++i)
      dst[i] = v[i];
  }

  template <>
  TSIMD_INLINE void store(const vdouble4 &v, void *_dst, const vboold4 &mask)
  {
    store(select(mask, v, load<vdouble4>(_dst)), _dst);
  }

  template <>
  TSIMD_INLINE void store(const vllong4 &v, void *_dst)
  {
    auto *dst = (typename vllong4::element_t *)_dst;

    for (int i = 0; i < 4; ++i)
      dst[i] = v[i];
  }

  template <>
  TSIMD_INLINE void store(const vllong4 &v, void *_dst, const vboold4 &mask)
  {
    store(select(mask, v, load<vllong4>(_dst)), _dst);
  }

  // 8-wide //

  template <>
  TSIMD_INLINE void store(const vfloat8 &v, void *_dst)
  {
#if defined(__AVX2__) || defined(__AVX__)
    return _mm256_store_ps((float *)_dst, v);
#else
    auto *dst = (typename vfloat8::element_t *)_dst;
    store(vfloat4(v.vl), dst);
    store(vfloat4(v.vh), dst + 4);
#endif
  }

  template <>
  TSIMD_INLINE void store(const vfloat8 &v, void *_dst, const vboolf8 &mask)
  {
#if defined(__AVX512VL__)
    _mm256_mask_store_ps((float*)_dst, mask, v);
#elif defined(__AVX2__) || defined(__AVX__)
    _mm256_maskstore_ps((float *)_dst, mask, v);
#else
    auto *dst = (typename vfloat8::element_t *)_dst;
    store(vfloat4(v.vl), dst, vboolf4(mask.vl));
    store(vfloat4(v.vh), dst + 4, vboolf4(mask.vh));
#endif
  }

  template <>
  TSIMD_INLINE void store(const vint8 &v, void *_dst)
  {
#if defined(__AVX2__)
    _mm256_store_si256((__m256i *)_dst, v);
#elif defined(__AVX__)
    _mm256_store_ps((float *)_dst, v);
#else
    auto *dst = (typename vint8::element_t *)_dst;
    store(vint4(v.vl), dst);
    store(vint4(v.vh), dst + 4);
#endif
  }

  template <>
  TSIMD_INLINE void store(const vint8 &v, void *_dst, const vboolf8 &mask)
  {
#if defined(__AVX512VL__)
    _mm256_mask_store_epi32(_dst, mask, v);
#elif defined(__AVX2__)
    _mm256_maskstore_epi32((int*)_dst, mask, v);
#elif defined(__AVX__)
    _mm256_maskstore_ps((float *)_dst, mask, v);
#else
    auto *dst = (typename vint8::element_t *)_dst;
    store(vint4(v.vl), dst, vboolf4(mask.vl));
    store(vint4(v.vh), dst + 4, vboolf4(mask.vh));
#endif
  }

  template <>
  TSIMD_INLINE void store(const vdouble8 &v, void *_dst)
  {
    auto *dst = (typename vdouble8::element_t *)_dst;
    store(vdouble4(v.vl), dst);
    store(vdouble4(v.vh), dst + 4);
  }

  template <>
  TSIMD_INLINE void store(const vdouble8 &v, void *_dst, const vboold8 &mask)
  {
    auto *dst = (typename vdouble8::element_t *)_dst;
    store(vdouble4(v.vl), dst, vboold4(mask.vl));
    store(vdouble4(v.vh), dst + 4, vboold4(mask.vh));
  }

  template <>
  TSIMD_INLINE void store(const vllong8 &v, void *_dst)
  {
    auto *dst = (typename vllong8::element_t *)_dst;
    store(vllong4(v.vl), dst);
    store(vllong4(v.vh), dst + 4);
  }

  template <>
  TSIMD_INLINE void store(const vllong8 &v, void *_dst, const vboold8 &mask)
  {
    auto *dst = (typename vllong8::element_t *)_dst;
    store(vllong4(v.vl), dst, vboold4(mask.vl));
    store(vllong4(v.vh), dst + 4, vboold4(mask.vh));
  }

  // 16-wide //

  template <>
  TSIMD_INLINE void store(const vfloat16 &v, void *_dst)
  {
#if defined(__AVX512F__)
    _mm512_store_ps((float *)_dst, v);
#else
    auto *dst = (typename vfloat16::element_t *)_dst;
    store(vfloat8(v.vl), dst);
    store(vfloat8(v.vh), dst + 8);
#endif
  }

  template <>
  TSIMD_INLINE void store(const vfloat16 &v, void *_dst, const vboolf16 &mask)
  {
#if defined(__AVX512F__)
    _mm512_mask_store_ps((float *)_dst, mask, v);
#else
    auto *dst = (typename vfloat16::element_t *)_dst;
    store(vfloat8(v.vl), dst, vboolf8(mask.vl));
    store(vfloat8(v.vh), dst + 8, vboolf8(mask.vh));
#endif
  }

  template <>
  TSIMD_INLINE void store(const vint16 &v, void *_dst)
  {
#if defined(__AVX512F__)
    _mm512_store_si512(_dst, v);
#else
    auto *dst = (typename vint16::element_t *)_dst;
    store(vint8(v.vl), dst);
    store(vint8(v.vh), dst + 8);
#endif
  }

  template <>
  TSIMD_INLINE void store(const vint16 &v, void *_dst, const vboolf16 &mask)
  {
#if defined(__AVX512F__)
    _mm512_mask_store_epi32(_dst, mask, v);
#else
    auto *dst = (typename vint16::element_t *)_dst;
    store(vint8(v.vl), dst, vboolf8(mask.vl));
    store(vint8(v.vh), dst + 8, vboolf8(mask.vh));
#endif
  }

  template <>
  TSIMD_INLINE void store(const vdouble16 &v, void *_dst)
  {
    auto *dst = (typename vdouble16::element_t *)_dst;
    store(vdouble8(v.vl), dst);
    store(vdouble8(v.vh), dst + 8);
  }

  template <>
  TSIMD_INLINE void store(const vdouble16 &v, void *_dst, const vboold16 &mask)
  {
    auto *dst = (typename vdouble16::element_t *)_dst;
    store(vdouble8(v.vl), dst, vboold8(mask.vl));
    store(vdouble8(v.vh), dst + 8, vboold8(mask.vh));
  }

  template <>
  TSIMD_INLINE void store(const vllong16 &v, void *_dst)
  {
    auto *dst = (typename vllong16::element_t *)_dst;
    store(vllong8(v.vl), dst);
    store(vllong8(v.vh), dst + 8);
  }

  template <>
  TSIMD_INLINE void store(const vllong16 &v, void *_dst, const vboold16 &mask)
  {
    auto *dst = (typename vllong16::element_t *)_dst;
    store(vllong8(v.vl), dst, vboold8(mask.vl));
    store(vllong8(v.vh), dst + 8, vboold8(mask.vh));
  }

}  // namespace tsimd