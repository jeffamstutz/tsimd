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

  // 1-wide //

  template <typename T>
  TSIMD_INLINE pack<T, 1> select(const pack<bool_t<T>, 1> &m,
                                 const pack<T, 1> &t,
                                 const pack<T, 1> &f)
  {
    return pack<T, 1>(m[0] ? t[0] : f[0]);
  }

  // 4-wide //

  TSIMD_INLINE vfloat4 select(const vboolf4 &m,
                              const vfloat4 &t,
                              const vfloat4 &f)
  {
#if defined(__SSE4_1__)
    return _mm_blendv_ps(f, t, m);
#elif defined(__SSE__)
    return _mm_or_ps(_mm_and_ps(m, t), _mm_andnot_ps(m, f));
#else
    vfloat4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = m[i] ? t[i] : f[i];

    return result;
#endif
  }

  TSIMD_INLINE vint4 select(const vboolf4 &m, const vint4 &t, const vint4 &f)
  {
#if defined(__SSE4_1__)
    return _mm_castps_si128(
        _mm_blendv_ps(_mm_castsi128_ps(f), _mm_castsi128_ps(t), m));
#elif defined(__SSE__)
    return _mm_or_si128(_mm_and_si128(m, t), _mm_andnot_si128(m, f));
#else
    vint4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = m[i] ? t[i] : f[i];

    return result;
#endif
  }

  // 8-wide //

  TSIMD_INLINE vfloat8 select(const vboolf8 &m,
                              const vfloat8 &t,
                              const vfloat8 &f)
  {
#if defined(__AVX2__) || defined(__AVX__)
    return _mm256_blendv_ps(f, t, m);
#else
    return vfloat8(select(vboolf4(m.vl), vfloat4(t.vl), vfloat4(f.vl)),
                   select(vboolf4(m.vh), vfloat4(t.vh), vfloat4(f.vh)));
#endif
  }

  TSIMD_INLINE vint8 select(const vboolf8 &m, const vint8 &t, const vint8 &f)
  {
#if defined(__AVX512F__) || defined(__AVX2__) || defined(__AVX__)
    return _mm256_castps_si256(
        _mm256_blendv_ps(_mm256_castsi256_ps(f), _mm256_castsi256_ps(t), m));
#else
    return vint8(select(vboolf4(m.vl), vint4(t.vl), vint4(f.vl)),
                 select(vboolf4(m.vh), vint4(t.vh), vint4(f.vh)));
#endif
  }

  // 16-wide //

  TSIMD_INLINE vfloat16 select(const vboolf16 &m,
                               const vfloat16 &t,
                               const vfloat16 &f)
  {
#if defined(__AVX512F__)
    return _mm512_mask_blend_ps(m, f, t);
#else
    return vfloat16(select(vboolf8(m.vl), vfloat8(t.vl), vfloat8(f.vl)),
                    select(vboolf8(m.vh), vfloat8(t.vh), vfloat8(f.vh)));
#endif
  }

  TSIMD_INLINE vint16 select(const vboolf16 &m,
                             const vint16 &t,
                             const vint16 &f)
  {
#if defined(__AVX512F__)
    return _mm512_mask_or_epi32(f, m, t, t);
#else
    return vint16(select(vboolf8(m.vl), vint8(t.vl), vint8(f.vl)),
                  select(vboolf8(m.vh), vint8(t.vh), vint8(f.vh)));
#endif
  }

  // Inferred pack-scalar select //////////////////////////////////////////////

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::can_convert<OTHER_T, T>>
  TSIMD_INLINE pack<T, W> select(const mask<T, W> &m,
                                 const pack<T, W> &t,
                                 const OTHER_T &f)
  {
    return select(m, t, pack<T, W>(f));
  }

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::can_convert<OTHER_T, T>>
  TSIMD_INLINE pack<T, W> select(const mask<T, W> &m,
                                 const OTHER_T &t,
                                 const pack<T, W> &f)
  {
    return select(m, pack<T, W>(t), f);
  }

}  // namespace tsimd