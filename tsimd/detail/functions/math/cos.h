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

#include "floor.h"

namespace tsimd {

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> cos(const pack<T, W> &p)
  {
    pack<T, W> result;

    for (int i = 0; i < W; ++i)
      result[i] = std::cos(p[i]);

    return result;
  }

  template <int W>
  TSIMD_INLINE vfloatn<W> cos(const vfloatn<W> &p)
  {
    static const float piOverTwoVec = 1.57079637050628662109375;
    static const float twoOverPiVec = 0.636619746685028076171875;
    const auto scaled = p * twoOverPiVec;
    const auto kReal = floor(scaled);
    const auto k = vintn<W>(kReal);

    // Reduced range version of x
    const auto x = p - kReal * piOverTwoVec;

    const auto kMod4 = k & 3;
    const auto cosUseCos = ((kMod4 == 0) | (kMod4 == 2));
    const auto flipSign = ((kMod4 == 1) | (kMod4 == 2));

    static const float sinC2 = -0.16666667163372039794921875;
    static const float sinC4 = 8.333347737789154052734375e-3;
    static const float sinC6 = -1.9842604524455964565277099609375e-4;
    static const float sinC8 = 2.760012648650445044040679931640625e-6;
    static const float sinC10 = -2.50293279435709337121807038784027099609375e-8;

    static const float cosC2 = -0.5;
    static const float cosC4 = 4.166664183139801025390625e-2;
    static const float cosC6 = -1.388833043165504932403564453125e-3;
    static const float cosC8 = 2.47562347794882953166961669921875e-5;
    static const float cosC10 = -2.59630184018533327616751194000244140625e-7;

    const auto outside = select(cosUseCos, 1., x);
    const auto c2 = select(cosUseCos, vfloatn<W>(cosC2), vfloatn<W>(sinC2));
    const auto c4 = select(cosUseCos, vfloatn<W>(cosC4), vfloatn<W>(sinC4));
    const auto c6 = select(cosUseCos, vfloatn<W>(cosC6), vfloatn<W>(sinC6));
    const auto c8 = select(cosUseCos, vfloatn<W>(cosC8), vfloatn<W>(sinC8));
    const auto c10 = select(cosUseCos, vfloatn<W>(cosC10), vfloatn<W>(sinC10));

    const auto x2 = x * x;
    auto formula = x2 * c10 + c8;
    formula = x2 * formula + c6;
    formula = x2 * formula + c4;
    formula = x2 * formula + c2;
    formula = x2 * formula + 1.;
    formula *= outside;

    formula = select(flipSign, -formula, formula);
    return formula;
  }

#if TSIMD_COMPILER_INTEL

#if defined(__SSE4_2__)
  TSIMD_INLINE vfloat4 cos(const vfloat4 &p)
  {
    return _mm_cos_ps(p);
  }
#endif

#if defined(__AVX__)
  TSIMD_INLINE vdouble4 cos(const vdouble4 &p)
  {
    return _mm256_cos_pd(p);
  }

  TSIMD_INLINE vfloat8 cos(const vfloat8 &p)
  {
    return _mm256_cos_ps(p);
  }
#endif

#if defined(__AVX512F__)
  TSIMD_INLINE vdouble8 cos(const vdouble8 &p)
  {
    return _mm512_cos_pd(p);
  }

  TSIMD_INLINE vfloat16 cos(const vfloat16 &p)
  {
    return _mm512_cos_ps(p);
  }
#endif

#endif

}  // namespace tsimd
