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

  // store() //////////////////////////////////////////////////////////////////

  template <typename PACK_T>
  TSIMD_INLINE void store(const PACK_T &p, void *_dst);

  template <typename PACK_T>
  TSIMD_INLINE void store(const PACK_T &p,
                          void *_dst,
                          const mask<typename PACK_T::value_t,
                                     PACK_T::static_size> &m);

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

  // TODO

  template <>
  TSIMD_INLINE void store(const vfloat4 &v, void *_dst)
  {
#if defined(__AVX512__) || defined(__AVX2__) || defined(__AVX__) || \
    defined(__SSE__)
    _mm_store_ps((float *)_dst, v);
#else
    auto *dst = (typename vfloat4::value_t *)_dst;

    for (int i = 0; i < 4; ++i)
      dst[i] = v[i];
#endif
  }

  // 8-wide //

  template <>
  TSIMD_INLINE void store(const vfloat8 &v, void *_dst)
  {
#if defined(__AVX512__) || defined(__AVX2__) || defined(__AVX__)
    return _mm256_store_ps((float *)_dst, v);
#else
    auto *dst = (typename vfloat8::value_t *)_dst;

    for (int i = 0; i < 8; ++i)
      dst[i] = v[i];
#endif
  }

  template <>
  TSIMD_INLINE void store(const vfloat8 &v, void *_dst, const vboolf8 &mask)
  {
#if defined(__AVX512__) || defined(__AVX2__) || defined(__AVX__)
    _mm256_maskstore_ps((float *)_dst, _mm256_castps_si256(mask), v);
#else
    auto *dst = (typename vfloat8::value_t *)_dst;

    for (int i = 0; i < 8; ++i)
      if (mask[i])
        dst[i] = v[i];
#endif
  }

  template <>
  TSIMD_INLINE void store(const vint8 &v, void *_dst)
  {
#if defined(__AVX512__) || defined(__AVX2__)
    _mm256_store_si256((__m256i *)_dst, v);
#elif defined(__AVX__)
    _mm256_store_ps((float *)_dst, _mm256_castsi256_ps(v));
#else
    auto *dst = (typename vint8::value_t *)_dst;

    for (int i = 0; i < 8; ++i)
      dst[i] = v[i];
#endif
  }

  template <>
  TSIMD_INLINE void store(const vint8 &v, void *_dst, const vboolf8 &mask)
  {
#if 0  // defined(__AVX512__) || defined(__AVX2__)
    _mm256_maskstore_epi32((int*)_dst, mask, v);
#elif defined(__AVX__)
    _mm256_maskstore_ps(
        (float *)_dst, _mm256_castps_si256(mask), _mm256_castsi256_ps(v));
#else
    auto *dst = (typename vint8::value_t *)_dst;

    for (int i = 0; i < 8; ++i)
      if (mask[i])
        dst[i] = v[i];
#endif
  }

  // 16-wide //

  template <>
  TSIMD_INLINE void store(const vfloat16 &v, void *_dst)
  {
#if defined(__AVX512__)
    _mm512_store_ps((float*)_dst, v); 
#else
    auto *dst = (typename vfloat16::value_t *)_dst;

    for (int i = 0; i < 16; ++i)
      dst[i] = v[i];
#endif
  }

  template <>
  TSIMD_INLINE void store(const vfloat16 &v, void *_dst, const vboolf16 &mask)
  {
#if defined(__AVX512F__)
    _mm512_mask_store_ps((float*)_dst, mask, v); 
#else
    auto *dst = (typename vfloat16::value_t *)_dst;

    for (int i = 0; i < 16; ++i)
      if (mask[i])
        dst[i] = v[i];
#endif
  }

  template <>
  TSIMD_INLINE void store(const vint16 &v, void *_dst)
  {
#if defined(__AVX512F__)
    _mm512_store_si512(_dst, v); 
#else
    auto *dst = (typename vint16::value_t *)_dst;

    for (int i = 0; i < 16; ++i)
      dst[i] = v[i];
#endif
  }

  template <>
  TSIMD_INLINE void store(const vint16 &v, void *_dst, const vboolf16 &mask)
  {
#if defined(__AVX512F__)
    _mm512_mask_store_epi32(_dst, mask, v); 
#else
    auto *dst = (typename vint16::value_t *)_dst;

    for (int i = 0; i < 16; ++i)
      if (mask[i])
        dst[i] = v[i];
#endif
  }

}  // namespace tsimd