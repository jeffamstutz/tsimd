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
  struct pack;

  template <typename OTHER_T, typename T, int W>
  pack<OTHER_T, W> convert_elements_to(const pack<T, W> &from);

  template <typename T, int W>
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

    template <typename OT, typename = traits::is_not_same_t<T, OT>>
    explicit pack(const pack<OT, W> &other)
    {
      *this = convert_elements_to<T>(other);
    }

    // (ugly syntax here) --> construct from intrinsic_t by value
    // NOTE(jda) - must define here because of MSVC...
    template <typename IT = intrinsic_t>
    pack(traits::enable_if_t<W != 1, IT> value) : v(value) {}

    // (ugly syntax here) --> construct from 2 x half_intrinsic_t by value
    // NOTE(jda) - must define here because of MSVC...
    template <typename IT = half_intrinsic_t>
    pack(traits::enable_if_t<!traits::half_simd_is_array<T, W>::value, IT> a,
         half_intrinsic_t b) : vl(a), vh(b) {}

    explicit pack(const std::array<T, W / 2> &a, const std::array<T, W / 2> &b);
    explicit pack(const std::array<T, W> &arr);

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

    operator const std::array<T, W> &() const;
    operator std::array<T, W> &();

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

  /* width-independant shortcuts */
  template <int W> using vfloatn = pack<float, W>;
  template <int W> using vintn   = pack<int, W>;
  template <int W> using vuintn  = pack<unsigned int, W>;
  template <int W> using vboolfn = maskf<W>;

  template <int W> using vdoublen = pack<double, W>;
  template <int W> using vllongn  = pack<long long, W>;
  template <int W> using vbooldn  = maskd<W>;

  /* 1-wide shortcuts */
  using vfloat1  = vfloatn<1>;
  using vint1    = vintn<1>;
  using vuint1   = vuintn<1>;
  using vboolf1  = maskf<1>;

  using vllong1  = vllongn<1>;
  using vdouble1 = vdoublen<1>;
  using vboold1  = maskd<1>;

  /* 4-wide shortcuts */
  using vfloat4  = vfloatn<4>;
  using vint4    = vintn<4>;
  using vuint4   = vuintn<4>;
  using vboolf4  = maskf<4>;

  using vdouble4 = vdoublen<4>;
  using vllong4  = vllongn<4>;
  using vboold4  = maskd<4>;

  /* 8-wide shortcuts */
  using vfloat8  = vfloatn<8>;
  using vint8    = vintn<8>;
  using vuint8   = vuintn<8>;
  using vboolf8  = maskf<8>;

  using vdouble8 = vdoublen<8>;
  using vllong8  = vllongn<8>;
  using vboold8  = maskd<8>;

  /* 16-wide shortcuts */
  using vfloat16  = vfloatn<16>;
  using vint16    = vintn<16>;
  using vuint16   = vuintn<16>;
  using vboolf16  = maskf<16>;

  using vdouble16 = vdoublen<16>;
  using vllong16  = vllongn<16>;
  using vboold16  = maskd<16>;

  /* default shortcuts */
  using vfloat  = vfloatn<TSIMD_DEFAULT_WIDTH>;
  using vint    = vintn<TSIMD_DEFAULT_WIDTH>;
  using vuint   = vuintn<TSIMD_DEFAULT_WIDTH>;
  using vboolf  = maskf<TSIMD_DEFAULT_WIDTH>;

#if TSIMD_DEFAULT_WIDTH > 1
  using vdouble = vdoublen<TSIMD_DEFAULT_WIDTH / 2>;
  using vllong  = vllongn<TSIMD_DEFAULT_WIDTH / 2>;
  using vboold  = maskd<TSIMD_DEFAULT_WIDTH / 2>;
#else
  using vdouble = vdouble1;
  using vllong  = vllong1;
  using vboold  = maskd1;
#endif

  // pack<> inlined members ///////////////////////////////////////////////////

  // pack<>::pack(T value) + specializations //

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(T value)
  {
#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      arr[i] = value;
  }

  // 4-wide //

#if defined(__SSE__)
  template <>
  TSIMD_INLINE vfloat4::pack(float value)
      : v(_mm_set1_ps(value))
  {
  }

  template <>
  TSIMD_INLINE vint4::pack(int value)
      : v(_mm_set1_epi32(value))
  {
  }
#endif

  // 8-wide //

#if defined(__AVX__)
  template <>
  TSIMD_INLINE vfloat8::pack(float value)
      : v(_mm256_set1_ps(value))
  {
  }

  template <>
  TSIMD_INLINE vint8::pack(int value)
      : v(_mm256_set1_epi32(value))
  {
  }
#endif

  // 16-wide //

#if defined(__AVX512F__)
  template <>
  TSIMD_INLINE vfloat16::pack(float value)
      : v(_mm512_set1_ps(value))
  {
  }

  template <>
  TSIMD_INLINE vint16::pack(int value)
      : v(_mm512_set1_epi32(value))
  {
  }
#endif

  // Generic pack<> members //

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(const std::array<T, W> &_arr) : arr(_arr)
  {
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(const std::array<T, W / 2> &a,
                                const std::array<T, W / 2> &b)
  {
    int i = 0;
    for (int j = 0; j < W / 2; j++, i++)
      arr[i] = a[j];
    for (int j = 0; j < W / 2; j++, i++)
      arr[i] = b[j];
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
  TSIMD_INLINE pack<T, W>::operator const std::array<T, W> &() const
  {
    return arr;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::operator std::array<T, W> &()
  {
    return arr;
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
#if TSIMD_WIN
    return &arr[0];
#else
    return arr.begin();
#endif
  }

  template <typename T, int W>
  TSIMD_INLINE T *pack<T, W>::end()
  {
    return begin() + W;
  }

  template <typename T, int W>
  TSIMD_INLINE const T *pack<T, W>::begin() const
  {
#if TSIMD_WIN
    return &arr[0];
#else
    return arr.begin();
#endif
  }

  template <typename T, int W>
  TSIMD_INLINE const T *pack<T, W>::end() const
  {
    return begin() + W;
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

  // pack<> cast definition ///////////////////////////////////////////////////

  template <typename OTHER_T, typename T, int W>
  TSIMD_INLINE pack<OTHER_T, W> convert_elements_to(const pack<T, W> &from)
  {
    pack<OTHER_T, W> to;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      to[i] = from[i];

    return to;
  }

  // pack<> reinterpret_cast //////////////////////////////////////////////////

  //NOTE(jda) - ugly syntax here --> The return type is a
  //            std::enable_if selecting when we can do an intrisic cast vs.
  //            having to manually do a reinterpret cast loop...both do the same
  //            thing.

  //NOTE(jda) - 'FROM_TYPE' and 'W' are implied from function parameter, user
  //            provides 'TO_TYPE' as a template param
  template <typename TO_TYPE, typename FROM_TYPE, int W>
  TSIMD_INLINE
  typename std::enable_if<
       traits::simd_type_is_native<TO_TYPE, W>::value, pack<TO_TYPE, W>>::type
  reinterpret_elements_as(const pack<FROM_TYPE, W> &p)
  {
    static_assert(!traits::is_bool<FROM_TYPE>::value,
                  "reinterpret_elements_as<> can't be done with masks!");
    static_assert(!traits::is_bool<TO_TYPE>::value,
                  "reinterpret_elements_as<> can't be done with masks!");
    static_assert(traits::same_size<FROM_TYPE, TO_TYPE>::value,
                  "reinterpret_elements_as<> only allows casting to element"
                  " element types of the same size!");
    static_assert(traits::valid_type_for_pack<TO_TYPE>::value,
                  "reinterpret_elements_as<> can only cast to valid pack"
                  " element types!");
    return pack<TO_TYPE, W>((const typename pack<TO_TYPE, W>::intrinsic_t)p);
  }

  //NOTE(jda) - 'FROM_TYPE' and 'W' are implied from function parameter, user
  //            provides 'TO_TYPE' as a template param
  template <typename TO_TYPE, typename FROM_TYPE, int W>
  TSIMD_INLINE
  typename std::enable_if<
      !traits::simd_type_is_native<TO_TYPE, W>::value, pack<TO_TYPE, W>>::type
  reinterpret_elements_as(const pack<FROM_TYPE, W> &p)
  {
    static_assert(!traits::is_bool<FROM_TYPE>::value,
                  "reinterpret_elements_as<> can't be done with masks!");
    static_assert(!traits::is_bool<TO_TYPE>::value,
                  "reinterpret_elements_as<> can't be done with masks!");
    static_assert(traits::same_size<FROM_TYPE, TO_TYPE>::value,
                  "reinterpret_elements_as<> only allows casting to element"
                  " element types of the same size!");
    static_assert(traits::valid_type_for_pack<TO_TYPE>::value,
                  "reinterpret_elements_as<> can only cast to valid pack"
                  " element types!");

    using T = const std::array<TO_TYPE, W>;
    return pack<TO_TYPE, W>(*(reinterpret_cast<T*>(p.begin())));
  }

}  // namespace tsimd