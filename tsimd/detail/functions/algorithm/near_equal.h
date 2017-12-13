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

#include "../math/abs.h"

namespace tsimd {

  #define EPSILON_DEFAULT TSIMD_DEFAULT_NEAR_EQUAL_EPSILON

  template <typename T, int W, typename = traits::is_floating_point_t<T>>
  TSIMD_INLINE mask<T, W> near_equal(const pack<T, W> &p1,
                                     const pack<T, W> &p2,
                                     T epsilon = EPSILON_DEFAULT)
  {
    return abs(p1 - p2) < epsilon;
  }

  template <typename T,
            typename U,
            int W,
            typename = traits::is_floating_point_t<T>>
  TSIMD_INLINE mask<T, W> near_equal(const pack<T, W> &p,
                                     U value,
                                     T epsilon = EPSILON_DEFAULT)
  {
    return near_equal(p, pack<T, W>(value), epsilon);
  }

  template <typename T,
            typename U,
            int W,
            typename = traits::is_floating_point_t<T>>
  TSIMD_INLINE mask<T, W> near_equal(U value,
                                     const pack<T, W> &p,
                                     T epsilon = EPSILON_DEFAULT)
  {
    return near_equal(pack<T, W>(value), p, epsilon);
  }

  #undef EPSILON_DEFAULT

}  // namespace tsimd