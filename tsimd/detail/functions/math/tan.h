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

#include <cmath>

#include "../../pack.h"

#include "../algorithm/select.h"
#include "../algorithm/set_if.h"

#include "cos.h"
#include "sin.h"

namespace tsimd {

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> tan(const pack<T, W> &p)
  {
    pack<T, W> result;

    for (int i = 0; i < W; ++i)
      result[i] = std::tan(p[i]);

    return result;
  }

  template <int W>
  TSIMD_INLINE vfloatn<W> tan(const vfloatn<W> &p)
  {
    static const float piOverFourVec = 0.785398185253143310546875;
    static const float fourOverPiVec = 1.27323949337005615234375;

    const auto xLt0 = p < 0.;
    const auto y = select(xLt0, -p, p);
    const auto scaled = y * fourOverPiVec;

    const auto kReal = floor(scaled);
    const auto k = vintn<W>(kReal);

    auto x = y - kReal * piOverFourVec;

    // If k & 1, x -= Pi/4
    const auto needOffset = (k & 1) != 0;
    set_if(x, x - piOverFourVec, needOffset);

    // If k & 3 == (0 or 3) let z = tan_In...(y) otherwise z = -cot_In0To...
    const auto kMod4 = k & 3;
    const auto useCotan = ((kMod4 == 1) | (kMod4 == 2));

    static const float oneVec = 1.0;

    static const float tanC2 = 0.33333075046539306640625;
    static const float tanC4 = 0.13339905440807342529296875;
    static const float tanC6 = 5.3348250687122344970703125e-2;
    static const float tanC8 = 2.46033705770969390869140625e-2;
    static const float tanC10 = 2.892402000725269317626953125e-3;
    static const float tanC12 = 9.500005282461643218994140625e-3;

    static const float cotC2 = -0.3333333432674407958984375;
    static const float cotC4 = -2.222204394638538360595703125e-2;
    static const float cotC6 = -2.11752182804048061370849609375e-3;
    static const float cotC8 = -2.0846328698098659515380859375e-4;
    static const float cotC10 = -2.548247357481159269809722900390625e-5;
    static const float cotC12 = -3.5257363606433500535786151885986328125e-7;

    const auto x2 = x * x;

    vfloatn<W> z;
    if (any(useCotan)) {
      auto cotVal = x2 * cotC12 + cotC10;
      cotVal = x2 * cotVal + cotC8;
      cotVal = x2 * cotVal + cotC6;
      cotVal = x2 * cotVal + cotC4;
      cotVal = x2 * cotVal + cotC2;
      cotVal = x2 * cotVal + oneVec;
      // The equation is for x * cot(x) but we need -x * cot(x) for the tan part
      cotVal /= -x;
      z = cotVal;
    }

    const auto useTan = vboolfn<W>(!useCotan);
    if (any(useTan)) {
      auto tanVal = x2 * tanC12 + tanC10;
      tanVal = x2 * tanVal + tanC8;
      tanVal = x2 * tanVal + tanC6;
      tanVal = x2 * tanVal + tanC4;
      tanVal = x2 * tanVal + tanC2;
      tanVal = x2 * tanVal + oneVec;
      // Equation was for tan(x)/x
      tanVal *= x;
      set_if(z, tanVal, useTan);
    }

    return select(xLt0, -z, z);
  }

#if TSIMD_COMPILER_INTEL

#if defined(__SSE4_2__)
  TSIMD_INLINE vfloat4 tan(const vfloat4 &p)
  {
    return _mm_tan_ps(p);
  }
#endif

#if defined(__AVX__)
  TSIMD_INLINE vdouble4 tan(const vdouble4 &p)
  {
    return _mm256_tan_pd(p);
  }

  TSIMD_INLINE vfloat8 tan(const vfloat8 &p)
  {
    return _mm256_tan_ps(p);
  }
#endif

#if defined(__AVX512F__)
  TSIMD_INLINE vdouble8 tan(const vdouble8 &p)
  {
    return _mm512_tan_pd(p);
  }

  TSIMD_INLINE vfloat16 tan(const vfloat16 &p)
  {
    return _mm512_tan_ps(p);
  }
#endif

#endif

}  // namespace tsimd
