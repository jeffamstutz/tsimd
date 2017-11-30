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

#include <algorithm>
#include <cmath>

#include "../pack.h"

#include "algorithm/any.h"

namespace tsimd {

  // set() ////////////////////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE void set(const mask<T, W>& mask,
                        pack<T, W>& a,
                        const pack<T, W>& b)
  {
      a = select(mask, b, a);
  }

  // abs() ////////////////////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> abs(const pack<T, W> &p)
  {
    pack<T, W> result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      result[i] = std::abs(p[i]);

    return result;
  }

  // sqrt() ///////////////////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> sqrt(const pack<T, W> &p)
  {
    pack<T, W> result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      result[i] = std::sqrt(p[i]);

    return result;
  }

  // floor() //////////////////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> floor(const pack<T, W> &p)
  {
    pack<T, W> result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      result[i] = std::floor(p[i]);

    return result;
  }

  // sin() ////////////////////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> sin(const pack<T, W> &p)
  {
    pack<T, W> result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      result[i] = std::sin(p[i]);

    return result;
  }

  // Transcendental functions from "ispc": https://github.com/ispc/ispc/
  // Most of the transcendental implementations in ispc code come from
  // Solomon Boulos's "syrah": https://github.com/boulos/syrah/

  template <int W>
  TSIMD_INLINE vfloatn<W> sin(const vfloatn<W> &p)
  {
    static const float piOverTwoVec = 1.57079637050628662109375;
    static const float twoOverPiVec = 0.636619746685028076171875;
    const auto scaled = p * twoOverPiVec;
    const auto kReal = floor(scaled);
    const auto k = vintn<W>(kReal);

    // Reduced range version of x
    const auto x = p - kReal * piOverTwoVec;
    const auto kMod4 = k & 3;
    const auto sinUseCos = (kMod4 == 1 || kMod4 == 3);
    const auto flipSign = (kMod4 > 1);

    // These coefficients are from sollya with fpminimax(sin(x)/x, [|0, 2,
    // 4, 6, 8, 10|], [|single...|], [0;Pi/2]);
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

    const auto outside = select(sinUseCos, 1.f, x);
    const auto c2 = select(sinUseCos, vfloatn<W>(cosC2), vfloatn<W>(sinC2));
    const auto c4 = select(sinUseCos, vfloatn<W>(cosC4), vfloatn<W>(sinC4));
    const auto c6 = select(sinUseCos, vfloatn<W>(cosC6), vfloatn<W>(sinC6));
    const auto c8 = select(sinUseCos, vfloatn<W>(cosC8), vfloatn<W>(sinC8));
    const auto c10 = select(sinUseCos, vfloatn<W>(cosC10), vfloatn<W>(sinC10));

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

  // cos() ////////////////////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> cos(const pack<T, W> &p)
  {
    pack<T, W> result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
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
    const auto cosUseCos = (kMod4 == 0 || kMod4 == 2);
    const auto flipSign = (kMod4 == 1 || kMod4 == 2);

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

  // tan() ////////////////////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> tan(const pack<T, W> &p)
  {
    pack<T, W> result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      result[i] = std::tan(p[i]);

    return result;
  }

#if 0 //NOTE(jda) - this comes from ispc, but generates a different answer to
      //            std::tan() on each value...
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
    set(needOffset, x, x - piOverFourVec);

    // If k & 3 == (0 or 3) let z = tan_In...(y) otherwise z = -cot_In0To...
    const auto kMod4 = k & 3;
    const auto useCotan = (kMod4 == 1) | (kMod4 == 2);

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
      set(useTan, z, tanVal);
    }

    return select(xLt0, -z, z);
  }
#endif

  // pow() ////////////////////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> pow(const pack<T, W> &v, const float b)
  {
    pack<T, W> result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      result[i] = std::pow(v[i], b);

    return result;
  }

  // max() ////////////////////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> max(const pack<T, W> &a, const pack<T, W> &b)
  {
    pack<T, W> result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      result[i] = std::max(a[i], b[i]);

    return result;
  }

  // min() ////////////////////////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> min(const pack<T, W> &a, const pack<T, W> &b)
  {
    pack<T, W> result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      result[i] = std::min(a[i], b[i]);

    return result;
  }

}  // namespace tsimd