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

#include "../memory/gather.h"

#include <random>

namespace tsimd {

  namespace detail {

    template <unsigned int BASE>
    TSIMD_INLINE float radicalInverse(unsigned int idx)
    {
      float f(0.f), g(1.0f), inv(1.0f/BASE);

      while (idx > 0) {
        g *= inv;
        f += (idx % BASE) * g;
        idx /= BASE;
      }

      return f;
    }

    template <>
    TSIMD_INLINE float radicalInverse<2>(unsigned int idx)
    {
      float f = 0.f, g = 1.0f;

      while (idx > 0) {
        g *= 0.5f;
        f += idx & 1 ? g : 0.f;
        idx >>= 1;
      }

      return f;
    }

  } // namespace detail

  template <int NUM_PRECOMPUTED, int SERIES_BASE, int W>
  struct precomputed_halton_engine
  {
    static_assert(SERIES_BASE >= 2,
                  "tsimd::precomputed_halton_engine 'SERIES_BASE' template"
                  " parameter (second one) must be >= 2.");

    precomputed_halton_engine();

    //NOTE(jda) - TODO: still need to be able to seed via constructor

    vfloatn<W> operator()();

    vfloatn<W> min() const;
    vfloatn<W> max() const;

  private:

    vintn<W> index{0};
    std::array<float, NUM_PRECOMPUTED> values;
  };

  #define NUM_DEFAULT 2048

  template <int W>
  using default_halton_engine2 = precomputed_halton_engine<NUM_DEFAULT, 2, W>;

  template <int W>
  using default_halton_engine3 = precomputed_halton_engine<NUM_DEFAULT, 3, W>;

  template <int W>
  using default_halton_engine5 = precomputed_halton_engine<NUM_DEFAULT, 5, W>;

  #undef NUM_DEFAULT

  // Inlined definitions //////////////////////////////////////////////////////

  template <int NUM_PRECOMPUTED, int SERIES_BASE, int W>
  TSIMD_INLINE
  precomputed_halton_engine<
    NUM_PRECOMPUTED,
    SERIES_BASE,
    W
  >
  ::precomputed_halton_engine()
  {
    for (int i = 0; i < NUM_PRECOMPUTED; i++)
      values[i] = detail::radicalInverse<SERIES_BASE>(i);

    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, NUM_PRECOMPUTED);

    for (int i = 0; i < W; i++)
      index[i] = dist(rd);
  };

  template <int NUM_PRECOMPUTED, int SERIES_BASE, int W>
  TSIMD_INLINE vfloatn<W>
  precomputed_halton_engine<NUM_PRECOMPUTED, SERIES_BASE, W>::operator()()
  {
    index = (index + 1) & (NUM_PRECOMPUTED - 1);
    return gather<vfloatn<W>>(values.data(), index);
  };

  template <int NUM_PRECOMPUTED, int SERIES_BASE, int W>
  TSIMD_INLINE vfloatn<W>
  precomputed_halton_engine<NUM_PRECOMPUTED, SERIES_BASE, W>::min() const
  {
    return vfloatn<W>(0.f);
  }

  template <int NUM_PRECOMPUTED, int SERIES_BASE, int W>
  TSIMD_INLINE vfloatn<W>
  precomputed_halton_engine<NUM_PRECOMPUTED, SERIES_BASE, W>::max() const
  {
    return vfloatn<W>(1.f);
  }

  // Function definitions /////////////////////////////////////////////////////

  template <int NUM_PRECOMPUTED, int SERIES_BASE, int W>
  TSIMD_INLINE vfloatn<W> generate_canonical(
    precomputed_halton_engine<NUM_PRECOMPUTED, SERIES_BASE, W> &engine)
  {
    return engine();
  }

} // namespace tsimd