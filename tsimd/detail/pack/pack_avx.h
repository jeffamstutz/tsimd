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

#include <array>
#include <iostream>

#include "../config.h"
#include "../tsimd_traits.h"

namespace tsimd {

  template <typename T>
  struct pack_avx : public detail::pack_base
  {
    // Compile-time info //

    static constexpr int static_size = 256 / sizeof(T);

    using element_t        = typename std::decay<T>::type;
    using intrinsic_t      = __m256;
    using half_intrinsic_t = __m128;
    using cast_intrinsic_t = __m256i;
    using array_t          = std::array<T, static_size>;
    using half_array_t     = std::array<T, static_size / 2>;
    using simd_or_array_t  = intrinsic_t;

    // Construction //

    pack_avx() = default;
    pack_avx(T value);

    // NOTE: only valid for W == 8! (otherwise it's a compile error)
    pack(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7);

    pack_avx(const pack_avx<T> &other);
    pack_avx(pack_avx<T> &&other);

    pack_avx& operator=(const pack<T> &other);
    pack_avx& operator=(pack<T> &&other) = default;

    template <typename OT, typename = traits::is_not_same_t<T, OT>>
    explicit pack_avx(const pack<OT, W> &other)
    {
      *this = convert_elements_to<T>(other);
    }

    pack_avx(intrinsic_t value) : v(value) {}
    pack_avx(half_intrinsic_t a, half_intrinsic_t b) : vl(a), vh(b) {}

    explicit pack_avx(const half_array_t &a, const half_array_t &b);
    explicit pack_avx(const array_t &arr);

    pack_avx<T> &operator=(const element_t &);

    template <typename OT, typename = traits::is_not_same_t<T, OT>>
    pack_avx<T> &operator=(const pack_avx<OT> &other)
    {
      return (*this = convert_elements_to<T>(other));
    }

    // Cast //

    template <typename OTHER_T>
    pack<OTHER_T, W> as();

    // Data conversions //

    operator const intrinsic_t &() const { return v; }
    operator intrinsic_t &()             { return v; }

    operator const cast_intrinsic_t &() const { return cv; }
    operator cast_intrinsic_t &()             { return cv; }

    operator const array_t &() const { return arr; }
    operator       array_t &()       { return arr; }

    explicit operator const T *() const { return arr.data(); }
    explicit operator T *()             { return arr.data(); }

    // Iterators //

    using iterator_t       = T*;
    using const_iterator_t = const T*;

    iterator_t begin() { return &arr[0]; }
    iterator_t end()   { return begin() + static_size; }

    const_iterator_t begin() const { return &arr[0]; }
    const_iterator_t end()   const { return begin() + static_size; }

    const_iterator_t cbegin() const { return begin(); }
    const_iterator_t cend()   const { return end(); }

    using iterator_deref_t       = decltype(*iterator_t());
    using const_iterator_deref_t = decltype(*const_iterator_t());

    // Element access //

    T extract(int i) const;

    void insert(T value, int i);

    const_iterator_deref_t operator[](int i) const { return *(begin() + i); }
    iterator_deref_t       operator[](int i)       { return *(begin() + i); }

    // Data //

    union
    {
      array_t arr;
      intrinsic_t v;
      struct
      {
        half_intrinsic_t vl, vh;
      };
    };
  };

  // pack<> inlined members ///////////////////////////////////////////////////

  template <typename T>
  TSIMD_INLINE pack_avx<T>::pack_avx(T value)
  {
    for (int i = 0; i < static_size; ++i)
      insert(value, i);
  }

  template <>
  TSIMD_INLINE pack_avx<float>::pack_avx(float value)
      : v(_mm256_set1_ps(value))
  {
  }

  template <>
  TSIMD_INLINE vint8::pack(int value)
      : v(_mm256_set1_epi32(value))
  {
  }

  template <typename T>
  TSIMD_INLINE
  pack_avx<T>::pack_avx(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7)
  {
    arr[0] = v0;
    arr[1] = v1;
    arr[2] = v2;
    arr[3] = v3;
    arr[4] = v4;
    arr[5] = v5;
    arr[6] = v6;
    arr[7] = v7;
  }

  template <typename T>
  TSIMD_INLINE pack_avx<T>::pack_avx(const pack_avx<T> &other)
      : pack_avx(static_cast<pack_avx<T>::simd_or_array_t>(other))
  {
  }

  template <typename T>
  TSIMD_INLINE pack_avx<T>::pack_avx(pack_avx<T> &&other)
      : pack(static_cast<pack_avx<T>::simd_or_array_t>(other))
  {
  }

  template <typename T>
  TSIMD_INLINE pack_avx<T> &pack_avx<T>::operator=(const pack_avx<T, W> &other)
  {
    return *this = pack<T, W>(static_cast<pack<T, W>::simd_or_array_t>(other));
  }

  template <typename T>
  TSIMD_INLINE pack<T, W>::pack(const typename pack<T, W>::array_t &_arr)
      : arr(_arr)
  {
  }

  template <typename T>
  TSIMD_INLINE pack<T, W>::pack(const typename pack<T, W>::half_array_t &a,
                                const typename pack<T, W>::half_array_t &b)
  {
    int i = 0;
    for (int j = 0; j < int(a.size()); j++, i++)
      arr[i] = a[j];
    for (int j = 0; j < int(b.size()); j++, i++)
      arr[i] = b[j];
  }

  template <typename T>
  TSIMD_INLINE pack<T, W> &pack<T, W>::operator=(const element_t &v)
  {
    *this = pack<T, W>(v);
    return *this;
  }

  template <typename T>
  TSIMD_INLINE T pack_avx<T>::extract(int i) const
  {
    return (*this)[i];
  }

  template <typename T>
  TSIMD_INLINE void pack_avx<T>::insert(T v, int i)
  {
    (*this)[i] = v;
  }

}  // namespace tsimd
