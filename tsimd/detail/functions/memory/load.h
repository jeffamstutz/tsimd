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

  // load() ///////////////////////////////////////////////////////////////////

  template <typename PACK_T>
  TSIMD_INLINE PACK_T load(const void *_src);

  template <typename PACK_T>
  TSIMD_INLINE PACK_T load(const void *_src,
                           const mask<typename PACK_T::value_t,
                                      PACK_T::static_size> &m);

  // 1-wide //

  namespace detail {
    template <
        typename PACK_T,
        typename = traits::is_pack_of_width_t<PACK_T, 1>>
    TSIMD_INLINE PACK_T load1(const void *_src)
    {
      using T = typename PACK_T::value_t;
      return PACK_T(*((T*)_src));
    }

    template <
        typename PACK_T,
        typename = traits::is_pack_of_width_t<PACK_T, 1>>
    TSIMD_INLINE PACK_T load1(const void *_src,
                              const mask_for_pack_t<PACK_T> &mask)
    {
      using T = typename PACK_T::value_t;
      return PACK_T(mask[0] ? *((T*)_src) : T());
    }
  } // ::tsimd::detail

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

  // TODO

  template <>
  TSIMD_INLINE vint4 load(const void *_src)
  {
#if defined(__AVX512__) || defined(__AVX__) || defined(__SSE__)
    return _mm_load_si128((const __m128i *)_src);
#else
    auto *src = (typename vint4::value_t *)_src;
    vint4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = src[i];

    return result;
#endif
  }

  // 8-wide //

  template <>
  TSIMD_INLINE vfloat8 load(const void *_src)
  {
#if defined(__AVX512__) || defined(__AVX__)
    return _mm256_load_ps((const float *)_src);
#else
    auto *src = (typename vfloat8::value_t *)_src;
    vfloat8 result;

    for (int i = 0; i < 8; ++i)
      result[i] = src[i];

    return result;
#endif
  }

  template <>
  TSIMD_INLINE vfloat8 load(const void *_src, const vboolf8 &mask)
  {
#if 0  // defined(__AVX512__)
    return _mm256_mask_load_ps(_mm256_setzero_ps(), mask, (float*)_src);
#elif defined(__AVX__)
    return _mm256_maskload_ps((const float *)_src, _mm256_castps_si256(mask));
#else
    auto *src = (typename vfloat8::value_t *)_src;
    vfloat8 result;

    for (int i = 0; i < 8; ++i)
      if (mask[i])
        result[i] = src[i];

    return result;
#endif
  }

  template <>
  TSIMD_INLINE vint8 load(const void *_src)
  {
#if defined(__AVX512__) || defined(__AVX__)
    return _mm256_castps_si256(_mm256_load_ps((const float *)_src));
#else
    auto *src = (typename vint8::value_t *)_src;
    vint8 result;

    for (int i = 0; i < 8; ++i)
      result[i] = src[i];

    return result;
#endif
  }

  template <>
  TSIMD_INLINE vint8 load(const void *_src, const vboolf8 &mask)
  {
#if 0  // defined(__AVX512__)
    return _mm256_castps_si256(_mm256_maskload_ps((float*)_src, mask));
#elif defined(__AVX__)
    return _mm256_castps_si256(
        _mm256_maskload_ps((const float *)_src, _mm256_castps_si256(mask)));
#else
    auto *src = (typename vint8::value_t *)_src;
    vint8 result;

    for (int i = 0; i < 8; ++i)
      if (mask[i])
        result[i] = src[i];

    return result;
#endif
  }

  // 16-wide //

  // TODO

}  // namespace tsimd