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

#include "../pack.h"

namespace tsimd {

  template <typename T, int W, typename FCN_T>
  TSIMD_INLINE void foreach (pack<T, W> &p, FCN_T && fcn)
  {
    for (int i = 0; i < W; ++i)
      fcn(p[i], i);
  }

  template <int W, typename FCN_T>
  TSIMD_INLINE void foreach_active(const mask<W> &m, FCN_T &&fcn)
  {
    for (int i = 0; i < W; ++i)
      if (m[i])
        fcn(i);
  }

  template <typename T, int W, typename FCN_T>
  TSIMD_INLINE void foreach_active(const mask<W> &m, pack<T, W> &p, FCN_T &&fcn)
  {
    for (int i = 0; i < W; ++i)
      if (m[i])
        fcn(p[i]);
  }

  // any() ////////////////////////////////////////////////////////////////////

  // 1-wide //

  TSIMD_INLINE bool any(const vboolf1 &a)
  {
    return a[0];
  }

  // 4-wide //

  // TODO

  // 8-wide //

  TSIMD_INLINE bool any(const vboolf8 &a)
  {
#if defined(__AVX512__) || defined(__AVX2__) || defined(__AVX__)
    return !_mm256_testz_ps(a, a);
#else
    NOT_YET_IMPLEMENTED;
#endif
  }

  // 16-wide //

  // TODO

  // none() ///////////////////////////////////////////////////////////////////

  template <int W>
  TSIMD_INLINE bool none(const mask<W> &m)
  {
    return !any(m);
  }

  // all() ////////////////////////////////////////////////////////////////////

  // 1-wide //

  TSIMD_INLINE bool all(const vboolf1 &a)
  {
    return any(a);
  }

  // 4-wide //

  // TODO

  // 8-wide //

  TSIMD_INLINE bool all(const vboolf8 &a)
  {
#if defined(__AVX512__) || defined(__AVX2__) || defined(__AVX__)
    return _mm256_movemask_ps(a) == (unsigned int)0xff;
#else
    NOT_YET_IMPLEMENTED;
#endif
  }

  // 16-wide //

  // TODO

  // select() /////////////////////////////////////////////////////////////////

  // 1-wide //

  template <typename T>
  TSIMD_INLINE pack<T, 1> select(const vboolf1 &m,
                                 const pack<T, 1> &t,
                                 const pack<T, 1> &f)
  {
    return pack<T, 1>(m[0] ? t[0] : f[0]);
  }

  // 4-wide //

  // TODO

  // 8-wide //

  TSIMD_INLINE vfloat8 select(const vboolf8 &m,
                              const vfloat8 &t,
                              const vfloat8 &f)
  {
#if defined(__AVX512__)
    return _mm256_mask_blend_ps(m, f, t);
#elif defined(__AVX2__) || defined(__AVX__)
    return _mm256_blendv_ps(f, t, m);
#else
    vfloat8 result;

    for (int i = 0; i < 8; ++i)
      result[i] = m[i] ? t[i] : f[i];

    return result;
#endif
  }

  TSIMD_INLINE vint8 select(const vboolf8 &m, const vint8 &t, const vint8 &f)
  {
#if defined(__AVX512__) || defined(__AVX2__) || defined(__AVX__)
    return _mm256_castps_si256(
        _mm256_blendv_ps(_mm256_castsi256_ps(f), _mm256_castsi256_ps(t), m));
#else
    vint8 result;

    for (int i = 0; i < 8; ++i)
      result[i] = m[i] ? t[i] : f[i];

    return result;
#endif
  }

  // 16-wide //

  // TODO

}  // namespace tsimd