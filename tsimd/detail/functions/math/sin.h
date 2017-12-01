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

#include <cmath>

#include "../../pack.h"

#include "../algorithm/any.h"
#include "../algorithm/select.h"
#include "../algorithm/set_if.h"

#include "fast_cos.h"
#include "fast_sin.h"

namespace tsimd {

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

  template <int W>
  TSIMD_INLINE vfloatn<W> sin(const vfloatn<W> &_p)
  {
    static const auto tpo2 = 3.f * M_PI / 2.f;
    static const auto two_pi = 2 * M_PI;

    auto p = select(_p < 0, abs(_p + two_pi), abs(_p));

    const auto two_pi_bucket = floor(p / two_pi);
    auto safe_p = p - (two_pi_bucket * two_pi);

    const auto quadrant_1 = safe_p <= M_PI_2;
    const auto quadrant_2 = safe_p > M_PI_2 && safe_p <= M_PI;
    const auto quadrant_3 = safe_p > M_PI && safe_p <= tpo2;
    const auto quadrant_4 = safe_p > tpo2 && safe_p <= two_pi;

    const auto pi_over_2_bucket = floor(safe_p / M_PI_2);
    safe_p = p - (pi_over_2_bucket * M_PI_2);

    vfloatn<W> result;

    if (any(quadrant_1) || any(quadrant_3)) {
      auto fast_sin_result = fast_sin(safe_p);
      set_if(result, fast_sin_result, quadrant_1);
      set_if(result, -fast_sin_result, quadrant_3);
    }

    if (any(quadrant_2) || any(quadrant_4)) {
      auto fast_cos_result = fast_cos(safe_p);
      set_if(result, fast_cos_result, quadrant_2);
      set_if(result, -fast_cos_result, quadrant_4);
    }

    return result;
  }

}  // namespace tsimd