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

#include <array>
#include <iostream>

#include "config.h"
#include "tsimd_traits.h"

namespace tsimd {

  template <typename T, int W = TSIMD_DEFAULT_WIDTH>
  struct pack
  {
    // Compile-time info //

    enum
    {
      static_size = W
    };
    using value_t          = typename std::decay<T>::type;
    using intrinsic_t      = typename traits::simd_type<value_t, W>::type;
    using half_intrinsic_t = typename traits::half_simd_type<value_t, W>::type;
    using cast_intrinsic_t = typename traits::cast_simd_type<value_t, W>::type;

    // Construction //

    pack() = default;
    explicit pack(T value);

    // (ugly syntax here) --> contstruct from intrinsic_t by value
    template <typename IT = intrinsic_t>
    pack(traits::enable_if_t<W != 1, IT> value);

    pack(half_intrinsic_t a, half_intrinsic_t b);

    pack<T, W> &operator=(const value_t &);

    // Array access //

    const T &operator[](int i) const;
    T &operator[](int i);

    // Cast //

    template <typename OTHER_T>
    pack<OTHER_T, W> as();

    // Data conversions //

    operator const intrinsic_t &() const;
    operator intrinsic_t &();

    operator const cast_intrinsic_t &() const;
    operator cast_intrinsic_t &();

    operator const T *() const;
    operator T *();

    // Iterators //

    T *begin();
    T *end();

    const T *begin() const;
    const T *end() const;

    const T *cbegin() const;
    const T *cend() const;

    // Data //

    union
    {
      std::array<T, W> arr;
      intrinsic_t v;
      cast_intrinsic_t cv;
      struct
      {
        half_intrinsic_t vl, vh;
      };
    };

    // Interface checks //

    static_assert(traits::valid_type_for_pack<T>::value,
                  "pack 'T' type currently must be 'float', 'int', 'double',"
                  " 'long long', 'bool32_t', or 'bool64_t'!");

    static_assert(W == 1 || W == 4 || W == 8 || W == 16,
                  "pack 'W' size must be 1, 4, 8, or 16!");
  };

  // mask types and true/false value aliases //////////////////////////////////

  template <typename T>
  using bool_t = typename traits::bool_type_for<T>::type;

  template <typename T, int W = TSIMD_DEFAULT_WIDTH>
  using mask = pack<bool_t<T>, W>;

  template <int W = TSIMD_DEFAULT_WIDTH>
  using maskf = pack<bool32_t, W>;

  template <int W = TSIMD_DEFAULT_WIDTH>
  using maskd = pack<bool64_t, W>;

  // Mask type for a given pack ///////////////////////////////////////////////

  template <typename MASK_T>
  struct mask_for_pack
  {
    using type = pack<bool_t<typename MASK_T::value_t>, MASK_T::static_size>;
  };

  template <typename MASK_T>
  using mask_for_pack_t = typename mask_for_pack<MASK_T>::type;

  // pack<> aliases ///////////////////////////////////////////////////////////

  /* 1-wide shortcuts */
  using vfloat1  = pack<float, 1>;
  using vdouble1 = pack<double, 1>;
  using vint1    = pack<int, 1>;
  using vuint1   = pack<unsigned int, 1>;
  using vllong1  = pack<long long, 1>;
  using vboolf1  = maskf<1>;
  using vboold1  = maskd<1>;

  /* 4-wide shortcuts */
  using vfloat4  = pack<float, 4>;
  using vdouble4 = pack<double, 4>;
  using vint4    = pack<int, 4>;
  using vuint4   = pack<unsigned int, 4>;
  using vllong4  = pack<long long, 4>;
  using vboolf4  = maskf<4>;
  using vboold4  = maskd<4>;

  /* 8-wide shortcuts */
  using vfloat8  = pack<float, 8>;
  using vdouble8 = pack<double, 8>;
  using vint8    = pack<int, 8>;
  using vuint8   = pack<unsigned int, 8>;
  using vllong8  = pack<long long, 8>;
  using vboolf8  = maskf<8>;
  using vboold8  = maskd<8>;

  /* 16-wide shortcuts */
  using vfloat16  = pack<float, 16>;
  using vdouble16 = pack<double, 16>;
  using vint16    = pack<int, 16>;
  using vuint16   = pack<unsigned int, 16>;
  using vllong16  = pack<long long, 16>;
  using vboolf16  = maskf<16>;
  using vboold16  = maskd<16>;

  /* default shortcuts */
  using vfloat  = pack<float, TSIMD_DEFAULT_WIDTH>;
  using vdouble = pack<double, TSIMD_DEFAULT_WIDTH>;
  using vint    = pack<int, TSIMD_DEFAULT_WIDTH>;
  #if TSIMD_DEFAULT_WIDTH > 1
  using vuint   = pack<unsigned int, TSIMD_DEFAULT_WIDTH / 2>;
  using vllong  = pack<long long, TSIMD_DEFAULT_WIDTH / 2>;
  #else
  using vuint   = vuint1;
  using vllong  = vllong1;
  #endif
  using vboolf  = maskf<TSIMD_DEFAULT_WIDTH>;
  using vboold  = maskd<TSIMD_DEFAULT_WIDTH>;

  // pack<> inlined members ///////////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(T value)
  {
#pragma omp simd
    for (int i = 0; i < W; ++i)
      arr[i] = value;
  }

  template <typename T, int W>
  template <typename IT>
  TSIMD_INLINE pack<T, W>::pack(
    typename std::enable_if<W != 1, IT>::type value
  ) : v(value)
  {
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(pack<T, W>::half_intrinsic_t a,
                                pack<T, W>::half_intrinsic_t b)
      : vl(a), vh(b)
  {
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> &pack<T, W>::operator=(const value_t &v)
  {
    *this = pack<T, W>(v);
    return *this;
  }

  template <typename T, int W>
  TSIMD_INLINE const T &pack<T, W>::operator[](int i) const
  {
    return arr[i];
  }

  template <typename T, int W>
  TSIMD_INLINE T &pack<T, W>::operator[](int i)
  {
    return arr[i];
  }

  template <typename T, int W>
  template <typename OTHER_T>
  TSIMD_INLINE pack<OTHER_T, W> pack<T, W>::as()
  {
    pack<OTHER_T, W> result;

#pragma omp simd
    for (int i = 0; i < W; ++i)
      result[i] = arr[i];

    return result;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::operator const intrinsic_t &() const
  {
    return v;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::operator intrinsic_t &()
  {
    return v;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::operator const cast_intrinsic_t &() const
  {
    return cv;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::operator cast_intrinsic_t &()
  {
    return cv;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::operator const T *() const
  {
    return arr.data();
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::operator T *()
  {
    return arr.data();
  }

  template <typename T, int W>
  TSIMD_INLINE T *pack<T, W>::begin()
  {
    return arr.begin();
  }

  template <typename T, int W>
  TSIMD_INLINE T *pack<T, W>::end()
  {
    return arr.end();
  }

  template <typename T, int W>
  TSIMD_INLINE const T *pack<T, W>::begin() const
  {
    return arr.begin();
  }

  template <typename T, int W>
  TSIMD_INLINE const T *pack<T, W>::end() const
  {
    return arr.end();
  }

  template <typename T, int W>
  TSIMD_INLINE const T *pack<T, W>::cbegin() const
  {
    return arr.begin();
  }

  template <typename T, int W>
  TSIMD_INLINE const T *pack<T, W>::cend() const
  {
    return arr.end();
  }

  // pack<> debugging functions ///////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE std::ostream &operator<<(std::ostream &o, const pack<T, W> &p)
  {
    o << "{";

    for (const auto &v : p)
      o << " " << v;

    o << " }";

    return o;
  }

  template <typename T, int W>
  TSIMD_INLINE void print(const pack<T, W> &p)
  {
    std::cout << p << std::endl;
  }

}  // namespace tsimd