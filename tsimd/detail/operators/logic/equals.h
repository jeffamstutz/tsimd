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
#include "not.h"

namespace tsimd {

  // binary operator==() //////////////////////////////////////////////////////

  // 1-wide //

  // TODO

  // 4-wide //

  // TODO

  // 8-wide //

  TSIMD_INLINE vboolf8 operator==(const vfloat8 &p1, const vfloat8 &p2)
  {
#if defined(__AVX512__)
    return _mm256_cmp_ps_mask(p1, p2, _MM_CMPINT_EQ);
#elif defined(__AVX__)
    return _mm256_cmp_ps(p1, p2, _CMP_EQ_OQ);
#else
    vboolf8 result;

    for (int i = 0; i < 8; ++i)
      result[i] = (p1[i] == p2[i]) ? vtrue : vfalse;

    return result;
#endif
  }

  template <typename OTHER_T>
  TSIMD_INLINE vboolf8 operator==(const vfloat8 &p1, const OTHER_T &v)
  {
    return p1 == vfloat8(v);
  }

  template <typename OTHER_T>
  TSIMD_INLINE vboolf8 operator==(const OTHER_T &v, const vfloat8 &p1)
  {
    return vfloat8(v) == p1;
  }

  TSIMD_INLINE vboolf8 operator==(const vint8 &p1, const vint8 &p2)
  {
#if defined(__AVX512__) || defined(__AVX2__)
    return _mm256_castsi256_ps(_mm256_cmpeq_epi32(p1, p2));
#elif defined(__AVX__)
    return vboolf8(_mm_castsi128_ps(_mm_cmpeq_epi32(p1.vl, p2.vl)),
                   _mm_castsi128_ps(_mm_cmpeq_epi32(p1.vh, p2.vh)));
#else
    vboolf8 result;

    for (int i = 0; i < 8; ++i)
      result[i] = (p1[i] == p2[i]) ? vtrue : vfalse;

    return result;
#endif
  }

  template <typename OTHER_T>
  TSIMD_INLINE vboolf8 operator==(const vint8 &p1, const OTHER_T &v)
  {
    return p1 == vint8(v);
  }

  template <typename OTHER_T>
  TSIMD_INLINE vboolf8 operator==(const OTHER_T &v, const vint8 &p1)
  {
    return vint8(v) == p1;
  }

  // 16-wide //

  // TODO

  // binary operator!=() //////////////////////////////////////////////////////

  // 1-wide //

  // TODO

  // 4-wide //

  // TODO

  // 8-wide //

  TSIMD_INLINE vboolf8 operator!=(const vfloat8 &p1, const vfloat8 &p2)
  {
    return !(p1 == p2);
  }

  template <typename OTHER_T>
  TSIMD_INLINE vboolf8 operator!=(const vfloat8 &p1, const OTHER_T &v)
  {
    return p1 != vfloat8(v);
  }

  template <typename OTHER_T>
  TSIMD_INLINE vboolf8 operator!=(const OTHER_T &v, const vfloat8 &p1)
  {
    return vfloat8(v) != p1;
  }

  TSIMD_INLINE vboolf8 operator!=(const vint8 &p1, const vint8 &p2)
  {
    return !(p1 == p2);
  }

  template <typename OTHER_T>
  TSIMD_INLINE vboolf8 operator!=(const vint8 &p1, const OTHER_T &v)
  {
    return p1 != vint8(v);
  }

  template <typename OTHER_T>
  TSIMD_INLINE vboolf8 operator!=(const OTHER_T &v, const vint8 &p1)
  {
    return vint8(v) != p1;
  }

  // 16-wide //

  // TODO

}  // namespace tsimd
