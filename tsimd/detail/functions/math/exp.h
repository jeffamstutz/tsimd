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

#include "floor.h"

#include "../algorithm/select.h"

namespace tsimd {

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> exp(const pack<T, W> &p)
  {
    pack<T, W> result;

    for (int i = 0; i < W; ++i)
      result[i] = std::exp(p[i]);

    return result;
  }

  namespace detail {

    template <int W>
    TSIMD_INLINE vfloatn<W> ldexp(vfloatn<W> x, vintn<W> n)
    {
      vintn<W> ex(0x7F800000);
      vintn<W> ix = reinterpret_elements_as<int>(x);
      ex &= ix;              // extract old exponent;
      ix = ix & ~0x7F800000u;  // clear exponent
      n = (n << 23) + ex;
      ix |= n; // insert new exponent
      return reinterpret_elements_as<float>(ix);
    }

  } // namespace detail

  template <int W>
  TSIMD_INLINE vfloatn<W> fast_exp(vfloatn<W> p)
  {
    auto z = floor(1.44269504088896341f * p + 0.5f);
    p -= z * 0.693359375f;
    p -= z * -2.12194440e-4f;
    auto n = vintn<W>(z);

    z = p * p;
    z = (((((1.9875691500E-4f  * p + 1.3981999507E-3f) * p +
            8.3334519073E-3f) * p + 4.1665795894E-2f) * p +
          1.6666665459E-1f) * p + 5.0000001201E-1f) * z + p + 1.f;
    p = detail::ldexp(z, n);
    return p;
  }

#if 1
  template <int W>
  TSIMD_INLINE vfloatn<W> exp(const vfloatn<W> &p)
  {
    const vfloatn<W> ln2_part1(0.6931457519);
    const vfloatn<W> ln2_part2(1.4286067653e-6);
    const vfloatn<W> one_over_ln2(1.44269502162933349609375);

    vfloatn<W> scaled(p * one_over_ln2);
    auto k_real = floor(scaled);
    vintn<W> k(k_real);

    // Reduced range version of x
    auto x = p - k_real * ln2_part1;
    x -= k_real * ln2_part2;

    // These coefficients are for e^x in [0, ln(2)]
    const vfloatn<W> one(1.);
    const vfloatn<W> c2(0.4999999105930328369140625);
    const vfloatn<W> c3(0.166668415069580078125);
    const vfloatn<W> c4(4.16539050638675689697265625e-2);
    const vfloatn<W> c5(8.378830738365650177001953125e-3);
    const vfloatn<W> c6(1.304379315115511417388916015625e-3);
    const vfloatn<W> c7(2.7555381529964506626129150390625e-4);

    auto result = x * c7 + c6;
    result = x * result + c5;
    result = x * result + c4;
    result = x * result + c3;
    result = x * result + c2;
    result = x * result + one;
    result = x * result + one;

    // Compute 2^k (should differ for float and double, but I'll avoid
    // it for now and just do floats)
    const vintn<W> fpbias(127);
    auto biased_n = k + fpbias;
    auto overflow = k > fpbias;
    // Minimum exponent is -126, so if k is <= -127 (k + 127 <= 0)
    // we've got underflow. -127 * ln(2) -> -88.02. So the most
    // negative float input that doesn't result in zero is like -88.
    auto underflow = (biased_n <= 0);
    const vintn<W> InfBits(0x7f800000);
    biased_n = biased_n << 23;
    // Reinterpret this thing as float
    vfloatn<W> two_to_the_n = reinterpret_elements_as<float>(biased_n);
    // Handle both doubles and floats (hopefully eliding the copy for float)
    auto elemtype_2n = two_to_the_n;
    result *= elemtype_2n;
    result = select(overflow, reinterpret_elements_as<float>(InfBits), result);
    result = select(underflow, 0., result);
    return result;
  }
#else
  template <int W>
  TSIMD_INLINE vfloatn<W> exp(const vfloatn<W> &p)
  {
    return fast_exp(p);
  }
#endif

}  // namespace tsimd
