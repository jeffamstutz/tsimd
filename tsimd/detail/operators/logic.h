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

#include "../pack.h"

#include "logic/and.h"
#include "logic/equals.h"
#include "logic/less_than.h"
#include "logic/or.h"

namespace tsimd {

  // binary operator<=() //

  template <typename T, int W>
  TSIMD_INLINE mask<W> operator<=(const pack<T, W> &p1, const pack<T, W> &p2)
  {
    mask<W> result;

#pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = (p1[i] <= p2[i]);

    return result;
  }

  template <
      typename T,
      int W,
      typename OTHER_T,
      typename = traits::can_convert<OTHER_T, T>
  >
  TSIMD_INLINE mask<W> operator<=(const pack<T, W> &p1, const OTHER_T &v)
  {
    mask<W> result;

#pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = (p1[i] <= v);

    return result;
  }

  template <
      typename T,
      int W,
      typename OTHER_T,
      typename = traits::can_convert<OTHER_T, T>
  >
  TSIMD_INLINE mask<W> operator<=(const OTHER_T &v, const pack<T, W> &p1)
  {
    return pack<T, W>(v) <= p1;
  }

  // binary operator>() //

  template <typename T, int W>
  TSIMD_INLINE mask<W> operator>(const pack<T, W> &p1, const pack<T, W> &p2)
  {
    mask<W> result;

#pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = (p1[i] > p2[i]);

    return result;
  }

  template <
      typename T,
      int W,
      typename OTHER_T,
      typename = traits::can_convert<OTHER_T, T>
  >
  TSIMD_INLINE mask<W> operator>(const pack<T, W> &p1, const OTHER_T &v)
  {
    mask<W> result;

#pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = (p1[i] > v);

    return result;
  }

  template <
      typename T,
      int W,
      typename OTHER_T,
      typename = traits::can_convert<OTHER_T, T>
  >
  TSIMD_INLINE mask<W> operator>(const OTHER_T &v, const pack<T, W> &p1)
  {
    return pack<T, W>(v) > p1;
  }

  // binary operator>=() //

  template <typename T, int W>
  TSIMD_INLINE mask<W> operator>=(const pack<T, W> &p1, const pack<T, W> &p2)
  {
    mask<W> result;

#pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = (p1[i] >= p2[i]);

    return result;
  }

  template <
      typename T,
      int W,
      typename OTHER_T,
      typename = traits::can_convert<OTHER_T, T>
  >
  TSIMD_INLINE mask<W> operator>=(const pack<T, W> &p1, const OTHER_T &v)
  {
    mask<W> result;

#pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = (p1[i] >= v);

    return result;
  }

  template <
      typename T,
      int W,
      typename OTHER_T,
      typename = traits::can_convert<OTHER_T, T>
  >
  TSIMD_INLINE mask<W> operator>=(const OTHER_T &v, const pack<T, W> &p1)
  {
    return pack<T, W>(v) >= p1;
  }

}  // namespace tsimd