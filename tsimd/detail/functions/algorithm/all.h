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

#include "any.h"

namespace tsimd {

  // 1-wide //

  TSIMD_INLINE bool all(const vboolf1 &a)
  {
    return any(a);
  }

  TSIMD_INLINE bool all(const vboold1 &a)
  {
    return any(a);
  }

  // 4-wide //

  TSIMD_INLINE bool all(const vboolf4 &a)
  {
#if defined(__SSE4_2__)
    return _mm_movemask_ps(a) == 0xf;
#else
    for (int i = 0; i < 4; ++i) {
      if (!a[i])
        return false;
    }

    return true;
#endif
  }

  TSIMD_INLINE bool all(const vboold4 &a)
  {
    for (int i = 0; i < 4; ++i) {
      if (!a[i])
        return false;
    }

    return true;
  }

  // 8-wide //

  TSIMD_INLINE bool all(const vboolf8 &a)
  {
#if defined(__AVX512VL__)
    return a.v == 0xff;
#elif defined(__AVX2__) || defined(__AVX__)
    return _mm256_movemask_ps(a) == (unsigned int)0xff;
#else
    return all(vboolf4(a.vl)) && all(vboolf4(a.vh));
#endif
  }

  TSIMD_INLINE bool all(const vboold8 &a)
  {
#if defined(__AVX512F__)
    return a.v == 0xff;
#else
    return all(vboold4(a.vl)) && all(vboold4(a.vh));
#endif
  }

  // 16-wide //

  TSIMD_INLINE bool all(const vboolf16 &a)
  {
#if defined(__AVX512F__)
    return _mm512_kortestc(a, a) != 0;
#else
    return all(vboolf8(a.vl)) && all(vboolf8(a.vh));
#endif
  }

  TSIMD_INLINE bool all(const vboold16 &a)
  {
#if defined(__AVX512F__)
    return all(vboolf16(a.v));
#else
    return all(vboold8(a.vl)) && all(vboold8(a.vh));
#endif
  }

}  // namespace tsimd
