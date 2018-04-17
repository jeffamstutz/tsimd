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

namespace tsimd {

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> log(const pack<T, W> &p)
  {
    pack<T, W> result;

    for (int i = 0; i < W; ++i)
      result[i] = std::log(p[i]);

    return result;
  }

  namespace detail {
    // Range reduction for logarithms takes log(x) -> log(2^n * y) -> n
    // * log(2) + log(y) where y is the reduced range (usually in [1/2,
    // 1)).
    template <int W>
    TSIMD_INLINE void range_reduce_log(const vfloatn<W> &input,
                                       vfloatn<W> &reduced,
                                       vintn<W> &exponent)
    {
      auto int_version = reinterpret_elements_as<int>(input);

      // single precision = SEEE EEEE EMMM MMMM MMMM MMMM MMMM MMMM
      // exponent mask    = 0111 1111 1000 0000 0000 0000 0000 0000
      //                    0x7  0xF  0x8  0x0  0x0  0x0  0x0  0x0
      // non-exponent     = 1000 0000 0111 1111 1111 1111 1111 1111
      //                  = 0x8  0x0  0x7  0xF  0xF  0xF  0xF  0xF

      static const int nonexponent_mask = 0x807FFFFF;

      // We want the reduced version to have an exponent of -1 which is -1 + 127 after biasing or 126
      static const int exponent_neg1 = (126l << 23);
      // NOTE(boulos): We don't need to mask anything out since we know
      // the sign bit has to be 0. If it's 1, we need to return infinity/nan
      // anyway (log(x), x = +-0 -> infinity, x < 0 -> NaN).

      //This number is [0, 255] but it means [-127, 128]
      auto biased_exponent = int_version >> 23;

      // Treat the number as if it were 2^{e+1} * (1.m)/2
      auto offset_exponent = biased_exponent + 1;
      exponent = offset_exponent - 127; // get the real value

      // Blend the offset_exponent with the original input (do this in
      // int for now, until I decide if float can have & and &not)
      auto blended = (int_version & nonexponent_mask) | (exponent_neg1);
      reduced = reinterpret_elements_as<float>(blended);
    }

  } // namespace detail

#if 0 //TODO: convert this ispc version to tsimd
  template <int W>
  TSIMD_INLINE vfloatn<W> fast_log(const vfloatn<W> &p)
  {
    int e;
    p = frexp(p, &e);

    int x_smaller_SQRTHF = (0.707106781186547524f > p) ? 0xffffffff : 0;
    e += x_smaller_SQRTHF;
    int ix_add = intbits(p);
    ix_add &= x_smaller_SQRTHF;
    p += floatbits(ix_add) - 1.f;

    float z = p * p;
    float y =
        ((((((((7.0376836292E-2f * p
                + -1.1514610310E-1f) * p
               + 1.1676998740E-1f) * p
              + -1.2420140846E-1f) * p
             + 1.4249322787E-1f) * p
            + -1.6668057665E-1f) * p
           + 2.0000714765E-1f) * p
          + -2.4999993993E-1f) * p
         + 3.3333331174E-1f) * p * z;

    float fe = (float)e;
    y += fe * -2.12194440e-4;
    y -= 0.5f * z;
    z  = p + y;
    return z + 0.693359375 * fe;
  }
#endif

  template <int W>
  TSIMD_INLINE vfloatn<W> log(const vfloatn<W> &p)
  {
    const vintn<W> NaN_bits(0x7fc00000);
    const vintn<W> Neg_Inf_bits(0xFF800000);
    const auto NaN = reinterpret_elements_as<float>(NaN_bits);
    const auto neg_inf = reinterpret_elements_as<float>(Neg_Inf_bits);
    auto use_nan = p < 0;
    auto use_inf = p == 0;
    auto exceptional = use_nan | use_inf;
    const vfloatn<W> one(1.0f);

    auto patched = select(exceptional, one, p);

    vfloatn<W> reduced;
    vintn<W> exponent;
    detail::range_reduce_log(patched, reduced, exponent);

    const auto ln2 = 0.693147182464599609375f;

    auto x1 = one - reduced;
    const auto c1 = 0.50000095367431640625f;
    const auto c2 = 0.33326041698455810546875f;
    const auto c3 = 0.2519190013408660888671875f;
    const auto c4 = 0.17541764676570892333984375f;
    const auto c5 = 0.3424419462680816650390625f;
    const auto c6 = -0.599632322788238525390625f;
    const auto c7 = +1.98442304134368896484375f;
    const auto c8 = -2.4899270534515380859375f;
    const auto c9 = +1.7491014003753662109375f;

    auto result = x1 * c9 + c8;
    result = x1 * result + c7;
    result = x1 * result + c6;
    result = x1 * result + c5;
    result = x1 * result + c4;
    result = x1 * result + c3;
    result = x1 * result + c2;
    result = x1 * result + c1;
    result = x1 * result + one;

    // Equation was for -(ln(red)/(1-red))
    result *= -x1;
    result += vfloatn<W>(exponent) * ln2;

    return select(exceptional, select(use_nan, NaN, neg_inf), result);
  }

}  // namespace tsimd
