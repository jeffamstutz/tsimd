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

namespace tsimd {

  template <typename PACK_T, typename OFFSET_T>
  TSIMD_INLINE void scatter(const PACK_T &p,
                            void *_dst,
                            const pack<OFFSET_T, PACK_T::static_size> &o)
  {
    auto *dst = (typename PACK_T::element_t *)_dst;

    for (int i = 0; i < PACK_T::static_size; ++i)
      dst[o[i]] = p[i];
  }

  template <typename PACK_T, typename OFFSET_T>
  TSIMD_INLINE void scatter(
      const PACK_T &p,
      void *_dst,
      const pack<OFFSET_T, PACK_T::static_size> &o,
      const mask<typename PACK_T::element_t, PACK_T::static_size> &m)
  {
    auto *dst = (typename PACK_T::element_t *)_dst;

    for (int i = 0; i < PACK_T::static_size; ++i)
      if (m[i])
        dst[o[i]] = p[i];
  }

}  // namespace tsimd
