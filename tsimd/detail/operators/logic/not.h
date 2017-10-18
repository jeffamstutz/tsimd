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

  // unary operator!() ////////////////////////////////////////////////////////

  // 1-wide //

  TSIMD_INLINE vboolf1 operator!(const vboolf1 &m)
  {
    return vboolf1(!m[0]);
  }

  // 4-wide //

  // TODO

  // 8-wide //

  TSIMD_INLINE vboolf8 operator!(const vboolf8 &m)
  {
#if defined(__AVX512__) || defined(__AVX__)
    return _mm256_xor_ps(m, vboolf8(vtrue));
#else
    vboolf8 result;

    for (int i = 0; i < 8; ++i)
      result[i] = !m[i] ? vtrue : vfalse;

    return result;
#endif
  }

  // 16-wide //

  // TODO

}  // namespace tsimd