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

namespace tsimd {

  template <typename T, int W, typename FCN_T>
  inline void foreach(pack<T, W> &p, FCN_T &&fcn)
  {
    #pragma omp simd
    for (int i = 0; i < W; ++i)
      fcn(p[i], i);
  }

  template <int W, typename FCN_T>
  inline void foreach_active(const mask<W> &m, FCN_T &&fcn)
  {
    #pragma omp simd
    for (int i = 0; i < W; ++i)
      if (m[i])
        fcn(i);
  }

  template <typename T, int W, typename FCN_T>
  inline void foreach_active(const mask<W> &m, pack<T, W> &p, FCN_T &&fcn)
  {
    #pragma omp simd
    for (int i = 0; i < W; ++i)
      if (m[i])
        fcn(p[i]);
  }

  template <int W>
  inline bool any(const mask<W> &m)
  {
    bool result = false;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      if (m[i])
        result = true;

    return result;
  }

  template <int W>
  inline bool none(const mask<W> &m)
  {
    return !any(m);
  }

  template <int W>
  inline bool all(const mask<W> &m)
  {
    bool result = true;

    #pragma omp simd
    for (int i = 0; i < W; ++i)
      if (!m[i])
        result = false;

    return result;
  }

  template <typename T, int W>
  inline pack<T, W> select(const mask<W> &m,
                           const pack<T, W> &t,
                           const pack<T, W> &f)
  {
    pack<T, W> result;

    #pragma omp simd
    for (int i = 0; i < W; ++i) {
      if (m[i])
        result[i] = t[i];
      else
        result[i] = f[i];
    }

    return result;
  }

} // ::tsimd