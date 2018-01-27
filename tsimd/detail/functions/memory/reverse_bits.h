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

#include <cassert>

#include "../../pack.h"

namespace tsimd {

  // 1-wide //

  template<typename T, typename = traits::is_n_bytes_t<T, 4>>
  TSIMD_INLINE pack<T, 1> reverse_bits(const pack<T, 1> &p)
  {
#if defined(__GNUG__) || defined(__clang__)
    const uint32_t swapped =
        __builtin_bswap32(*reinterpret_cast<const uint32_t*>(&p.v));
#elif defined(_MSC_VER)
    const unsigned long swapped =
        _byteswap_ulong(*reinterpret_cast<const unsigned long*>(&p.v));
#else
#error "Unrecognized Compiler!"
#endif
    return pack<T, 1>(*reinterpret_cast<const T*>(&swapped));
  }

  // 4-wide //

  template<typename T, typename = traits::is_n_bytes_t<T, 4>>
  TSIMD_INLINE pack<T, 4> reverse_bits(const pack<T, 4> &p)
  {
#if defined(__SSSE3__)
    const __m128i mask =
        _mm_set_epi8(12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3);
    pack<T, 4> result(0);
    result.v = _mm_shuffle_epi8(p, mask);
    return result;
#else
    pack<T, 4> result;

    for (int i = 0; i < 4; ++i)
      result[i] = reverse_bits(pack<T, 1>(p[i]));

    return result;
#endif
  }

  // 8-wide //

  template<typename T, typename = traits::is_n_bytes_t<T, 4>>
  TSIMD_INLINE pack<T, 8> reverse_bits(const pack<T, 8> &p)
  {
#if defined(__AVX512__) || defined(__AVX2__)
    // The AVX shuffle is the same as the SSSE3 but just doubled up by splitting
    // the 256 bit vector in half, so the mask for each half is the same as in
    // the SSSE3 case
    const __m256i mask =
        _mm256_set_epi32(0x0c0d0e0f, 0x08090a0b, 0x04050607, 0x00010203,
                         0x0c0d0e0f, 0x08090a0b, 0x04050607, 0x00010203);
    pack<T, 8> result(0);
    result.v = _mm256_shuffle_epi8(p, mask);
    return result;
#else
    return pack<T, 8>(reverse_bits(pack<T, 4>(p.vl)),
                      reverse_bits(pack<T, 4>(p.vh)));
#endif
  }

  // 16-wide //

  template<typename T, typename = traits::is_n_bytes_t<T, 4>>
  TSIMD_INLINE pack<T, 16> reverse_bits(const pack<T, 16> &p)
  {
#if defined(__AVX512BW__)
    // The AVX512-BW shuffle is the same as the SSSE3 but just quadrupled up
    // by splitting the 512 bit vector in four, so the mask for each piece is
    // the same as in the SSSE3 case
    const __m512i mask =
        _mm512_set_epi32(0x0c0d0e0f, 0x08090a0b, 0x04050607, 0x00010203,
                         0x0c0d0e0f, 0x08090a0b, 0x04050607, 0x00010203,
                         0x0c0d0e0f, 0x08090a0b, 0x04050607, 0x00010203,
                         0x0c0d0e0f, 0x08090a0b, 0x04050607, 0x00010203);
    pack<T, 16> result(0);
    result.v = _mm512_shuffle_epi8(p, mask);
    return result;
#else
    return pack<T, 16>(reverse_bits(pack<T, 8>(p.vl)),
                       reverse_bits(pack<T, 8>(p.vh)));
#endif
  }
}

