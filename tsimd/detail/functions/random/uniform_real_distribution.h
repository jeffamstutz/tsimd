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

  // TODO: verify PACK_T is indeed a pack<>!
  template <typename PACK_T>
  struct uniform_real_distribution
  {
    uniform_real_distribution(const PACK_T &a, const PACK_T &b);

    uniform_real_distribution(typename PACK_T::element_t a,
                              typename PACK_T::element_t b);

    template <typename VRNG>
    PACK_T operator()(VRNG& generator);

    // property functions
    PACK_T a() const;
    PACK_T b() const;

    PACK_T min() const;
    PACK_T max() const;

  private:

    PACK_T _a, _b;
  };

  // Inlined definitions //////////////////////////////////////////////////////

  template <typename PACK_T>
  TSIMD_INLINE uniform_real_distribution<PACK_T>::uniform_real_distribution(
    const PACK_T &a,
    const PACK_T &b)
      : _a(a), _b(b)
  {
  }

  template <typename PACK_T>
  TSIMD_INLINE uniform_real_distribution<PACK_T>::uniform_real_distribution(
    typename PACK_T::element_t a,
    typename PACK_T::element_t b)
      : uniform_real_distribution(PACK_T(a), PACK_T(b))
  {
  }

  template <typename PACK_T>
  template <typename VRNG>
  TSIMD_INLINE PACK_T
  uniform_real_distribution<PACK_T>::operator()(VRNG& generator)
  {
    return (b() - a()) * PACK_T(generate_canonical(generator)) + a();
  }

  template <typename PACK_T>
  TSIMD_INLINE PACK_T uniform_real_distribution<PACK_T>::a() const
  {
    return _a;
  }

  template <typename PACK_T>
  TSIMD_INLINE PACK_T uniform_real_distribution<PACK_T>::b() const
  {
    return _b;
  }

  template <typename PACK_T>
  TSIMD_INLINE PACK_T uniform_real_distribution<PACK_T>::min() const
  {
    return a();
  }

  template <typename PACK_T>
  TSIMD_INLINE PACK_T uniform_real_distribution<PACK_T>::max() const
  {
    return b();
  }

} // namespace tsimd
