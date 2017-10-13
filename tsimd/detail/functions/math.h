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

namespace tsimd {

  template <typename T, int W>
  inline pack<T, W> abs(const pack<T, W> &p)
  {
    pack<T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = std::abs(p[i]);

    return result;
  }

  template <typename T, int W>
  inline pack<T, W> sqrt(const pack<T, W> &p)
  {
    pack<T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = std::sqrt(p[i]);

    return result;
  }

  template <typename T, int W>
  inline pack<T, W> sin(const pack<T, W> &p)
  {
    pack<T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = std::sin(p[i]);

    return result;
  }

  template <typename T, int W>
  inline pack<T, W> cos(const pack<T, W> &p)
  {
    pack<T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = std::cos(p[i]);

    return result;
  }

  template <typename T, int W>
  inline pack<T, W> tan(const pack<T, W> &p)
  {
    pack<T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = std::tan(p[i]);

    return result;
  }

  template <typename T, int W>
  inline pack<T, W> pow(const pack<T, W> &v, const float b)
  {
    pack<T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = std::pow(v[i], b);

    return result;
  }

  template <typename T, int W>
  inline pack<T, W> max(const pack<T, W> &a, const pack<T, W> &b)
  {
    pack<T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = std::max(a[i], b[i]);

    return result;
  }

  template <typename T, int W>
  inline pack<T, W> min(const pack<T, W> &a, const pack<T, W> &b)
  {
    pack<T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = std::min(a[i], b[i]);

    return result;
  }

} // ::tsimd