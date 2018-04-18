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

  template <typename T, typename = traits::is_bool_t<T>>
  TSIMD_INLINE mask<T, 1> operator!(const mask<T, 1> &m)
  {
    return mask<T, 1>(!m[0]);
  }

  // 4-wide //

  TSIMD_INLINE vboolf4 operator!(const vboolf4 &m)
  {
#if defined(__SSE4_2__)
    return _mm_xor_ps(m, vboolf4(true));
#else
    vboolf4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = !m[i];

    return result;
#endif
  }

  TSIMD_INLINE vboold4 operator!(const vboold4 &m)
  {
    vboold4 result;

    for (int i = 0; i < 4; ++i)
      result[i] = !m[i];

    return result;
  }

  // 8-wide //

  TSIMD_INLINE vboolf8 operator!(const vboolf8 &m)
  {
#if defined(__AVX512VL__)
    return _mm512_knot(m);
#elif defined(__AVX2__) || defined(__AVX__)
    return _mm256_xor_ps(m, vboolf8(true));
#else
    return vboolf8(!vboolf4(m.vl), !vboolf4(m.vh));
#endif
  }

  TSIMD_INLINE vboold8 operator!(const vboold8 &m)
  {
#if defined(__AVX512F__)
    return _mm512_knot(m);
#else
    return vboold8(!vboold4(m.vl), !vboold4(m.vh));
#endif
  }

  // 16-wide //

  TSIMD_INLINE vboolf16 operator!(const vboolf16 &m)
  {
#if defined(__AVX512F__)
    return _mm512_knot(m);
#else
    return vboolf16(!vboolf8(m.vl), !vboolf8(m.vh));
#endif
  }

  TSIMD_INLINE vboold16 operator!(const vboold16 &m)
  {
    return vboold16(!vboold8(m.vl), !vboold8(m.vh));
  }

}  // namespace tsimd