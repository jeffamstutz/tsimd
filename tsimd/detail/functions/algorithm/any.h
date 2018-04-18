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
  TSIMD_INLINE bool any(const pack<T, 1> &a)
  {
    return a[0];
  }

  // 4-wide //

  TSIMD_INLINE bool any(const vboolf4 &a)
  {
#if defined(__SSE4_2__)
    return _mm_movemask_ps(a) != 0x0;
#else
    for (int i = 0; i < 4; ++i) {
      if (a[i])
        return true;
    }

    return false;
#endif
  }

  TSIMD_INLINE bool any(const vboold4 &a)
  {
    for (int i = 0; i < 4; ++i) {
      if (a[i])
        return true;
    }

    return false;
  }

  // 8-wide //

  TSIMD_INLINE bool any(const vboolf8 &a)
  {
#if defined(__AVX512VL__)
    return _mm512_kortestz(a, a) == 0;
#elif defined(__AVX2__) || defined(__AVX__)
    return !_mm256_testz_ps(a, a);
#else
    return any(vboolf4(a.vl)) || any(vboolf4(a.vh));
#endif
  }

  TSIMD_INLINE bool any(const vboold8 &a)
  {
#if defined(__AVX512F__)
    return _mm512_kortestz(a, a) == 0;
#else
    return any(vboold4(a.vl)) || any(vboold4(a.vh));
#endif
  }

  // 16-wide //

  TSIMD_INLINE bool any(const vboolf16 &a)
  {
#if defined(__AVX512F__)
    return _mm512_kortestz(a, a) == 0;
#else
    return any(vboolf8(a.vl)) || any(vboolf8(a.vh));
#endif
  }

  TSIMD_INLINE bool any(const vboold16 &a)
  {
#if defined(__AVX512F__)
    return any(vboolf16(a.v));
#else
    return any(vboold8(a.vl)) || any(vboold8(a.vh));
#endif
  }

  // none() ///////////////////////////////////////////////////////////////////

  template <typename MASK_T, typename = traits::is_mask_t<MASK_T>>
  TSIMD_INLINE bool none(const MASK_T &m)
  {
    return !any(m);
  }

}  // namespace tsimd
