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

#include "../../pack.h"

#include "select.h"

namespace tsimd {

  template <typename T, int W, typename FCN_T>
  TSIMD_INLINE void foreach(pack<T, W> &p, FCN_T && fcn)
  {
    for (int i = 0; i < W; ++i)
      fcn(p[i], i);
  }

  template <typename BOOL_T,
            int W,
            typename FCN_T,
            typename = traits::is_bool_t<BOOL_T>>
  TSIMD_INLINE void foreach_active(const pack<BOOL_T, W> &m, FCN_T &&fcn)
  {
    for (int i = 0; i < W; ++i)
      if (m[i])
        fcn(i);
  }

  template <typename T, int W, typename FCN_T>
  TSIMD_INLINE void foreach_active(const mask<T, W> &m,
                                   pack<T, W> &p,
                                   FCN_T &&fcn)
  {
    for (int i = 0; i < W; ++i)
      if (m[i])
        fcn(p[i]);
  }

  template <typename T, int W, typename FCN_T>
  inline void foreach_unique(mask<T, W> mask,
                             const pack<T, W> &p,
                             FCN_T &&fcn)
  {
    for (int i = 0; i < W; i++) {
      if (!mask[i]) continue;
      const auto sValue = p[i];
      const auto matching = (mask & (p == sValue));
      fcn(matching, sValue);
      // TODO: not implemented: mask ^= matchingMask;
      // TODO: gives an error: mask = mask & ~matchingMask;
      mask = mask ^ matching;
    }
  }

  template <typename T, int W, typename FCN_T>
  inline void foreach_unique(const pack<T, W> &p, FCN_T &&fcn)
  {
    foreach_unique(mask<T, W>(true), p, fcn);
  }

}  // namespace tsimd
