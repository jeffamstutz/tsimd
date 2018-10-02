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

#include "config.h"
#include "tsimd_traits.h"

#include "utility/bit_iterator.h"

namespace tsimd {

  template <typename T, int W = TSIMD_DEFAULT_WIDTH>
  struct pack;

  template <typename OTHER_T, typename T, int W>
  TSIMD_INLINE pack<OTHER_T, W> convert_elements_to(const pack<T, W> &from);

  template <typename T, int W>
  struct pack : public detail::pack_base
  {
    // Compile-time info //

    enum
    {
      static_size = W
    };
    using element_t        = typename std::decay<T>::type;
    using intrinsic_t      = typename traits::simd_type<T, W>::type;
    using half_intrinsic_t = typename traits::half_simd_type<T, W>::type;
    using cast_intrinsic_t = typename traits::cast_simd_type<T, W>::type;

    // NOTE(jda) - This alias normally just represents a std::array<T, W> in
    //             the union below to allow for easy per-element access of the
    //             pack<>. However, masks (i.e. bool32_t and bool64_t) on AVX512
    //             are bit masks, meaning there isn't a good array
    //             representation...in that case, default to
    //             std::array<size_t, 0>.
    using array_t      = typename traits::array_for_pack<T, W>::type;
    using half_array_t = typename traits::half_array_for_pack<T, W>::type;

    // NOTE(jda) - This alias represents an "ideal" underlying representation,
    //             which will the underlying intrinsic type (if available) or
    //             fallback to a plain std::array<T, W>. Used for copy/move
    //             constructor optimizations done by the compiler.
    using simd_or_array_t = typename traits::simd_or_array_type<T, W>::type;

    // Construction //

    pack() = default;
    pack(T value);

    template <typename U, typename = traits::valid_type_for_pack_ctor_t<T, U>>
    explicit pack(U value) : pack(static_cast<T>(value)) {}

    // NOTE: let pack(bool) ctor work for vbool types (requires conversion)
    explicit pack(bool value) : pack(static_cast<T>(value)) {}

    // NOTE: only valid for W == 4! (otherwise it's a compile error)
    pack(T v0, T v1, T v2, T v3);

    // NOTE: only valid for W == 8! (otherwise it's a compile error)
    pack(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7);

    // NOTE: only valid for W == 16! (otherwise it's a compile error)
    pack(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7,
         T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15);

    pack(const pack<T, W> &other);
    pack(pack<T, W> &&other);

    pack& operator=(const pack<T, W> &other);
    pack& operator=(pack<T, W> &&other) = default;

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

    explicit pack(const half_array_t &a, const half_array_t &b);
    explicit pack(const array_t &arr);

    pack<T, W> &operator=(const element_t &);

    template <typename OT, typename = traits::is_not_same_t<T, OT>>
    pack<T, W> &operator=(const pack<OT, W> &other)
    {
      return (*this = convert_elements_to<T>(other));
    }

    // Cast //

    template <typename OTHER_T>
    pack<OTHER_T, W> as();

    // Data conversions //

    operator const intrinsic_t &() const;
    operator intrinsic_t &();

    operator const cast_intrinsic_t &() const;
    operator cast_intrinsic_t &();

    operator const array_t &() const;
    operator       array_t &();

    explicit operator const T *() const;
    explicit operator T *();

    // Iterators //

    using iterator_t       = typename traits::pack_iterator<T, W>::type;
    using const_iterator_t = typename traits::const_pack_iterator<T, W>::type;

    iterator_t begin();
    iterator_t end();

    const_iterator_t begin() const;
    const_iterator_t end() const;

    const_iterator_t cbegin() const;
    const_iterator_t cend() const;

    // NOTE(jda) - operator[]() returns different types based on whether
    //             'iterator_t' is just a pointer, or a bit_iterator (e.g.
    //             AVX512 masks)
    using iterator_deref_t       = decltype(*iterator_t());
    using const_iterator_deref_t = decltype(*const_iterator_t());

    // Element access //

    T extract(int i) const;

    void insert(T value, int i);

    const_iterator_deref_t operator[](int i) const;
    iterator_deref_t       operator[](int i);

    // Data //

    union
    {
      array_t arr;
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
    using type = pack<bool_t<typename MASK_T::element_t>, MASK_T::static_size>;
  };

  template <typename MASK_T>
  using mask_for_pack_t = typename mask_for_pack<MASK_T>::type;

  // pack<> aliases ///////////////////////////////////////////////////////////

  /* element-independant shortcuts */
  template <typename T> using vpack = pack<T, TSIMD_DEFAULT_WIDTH>;

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

  using vdouble = vdoublen<TSIMD_DEFAULT_WIDTH>;
  using vllong  = vllongn<TSIMD_DEFAULT_WIDTH>;
  using vboold  = maskd<TSIMD_DEFAULT_WIDTH>;

  // pack<> inlined members ///////////////////////////////////////////////////

  // pack<>::pack(T value) + specializations //

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(T value)
  {
    for (int i = 0; i < W; ++i)
      insert(value, i);
  }

  // 4-wide //

#if defined(__SSE4_2__)
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

  template <>
  TSIMD_INLINE vboolf16::pack(bool32_t value)
  {
    v = value ? 0xFFFF : 0x0000;
  }

  template <>
  TSIMD_INLINE vboold16::pack(bool64_t value)
  {
    v = value ? 0xFFFF : 0x0000;
  }
#endif

  // Generic pack<> members //

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(T v0, T v1, T v2, T v3)
  {
    static_assert(W == 4,
                  "Multi-scalar constructors take exactly W arguments");
    arr[0] = v0;
    arr[1] = v1;
    arr[2] = v2;
    arr[3] = v3;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7)
  {
    static_assert(W == 8,
                  "Multi-scalar constructors take exactly W arguments");
    arr[0] = v0;
    arr[1] = v1;
    arr[2] = v2;
    arr[3] = v3;
    arr[4] = v4;
    arr[5] = v5;
    arr[6] = v6;
    arr[7] = v7;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7,
                                T v8, T v9, T v10, T v11, T v12, T v13, T v14,
                                T v15)
  {
    static_assert(W == 16,
                  "Multi-scalar constructors take exactly W arguments");
    arr[0] = v0;
    arr[1] = v1;
    arr[2] = v2;
    arr[3] = v3;
    arr[4] = v4;
    arr[5] = v5;
    arr[6] = v6;
    arr[7] = v7;
    arr[8] = v8;
    arr[9] = v9;
    arr[10] = v10;
    arr[11] = v11;
    arr[12] = v12;
    arr[13] = v13;
    arr[14] = v14;
    arr[15] = v15;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(const pack<T, W> &other)
      : pack(static_cast<pack<T, W>::simd_or_array_t>(other))
  {
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(pack<T, W> &&other)
      : pack(static_cast<pack<T, W>::simd_or_array_t>(other))
  {
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> &pack<T, W>::operator=(const pack<T, W> &other)
  {
    return *this = pack<T, W>(static_cast<pack<T, W>::simd_or_array_t>(other));
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(const typename pack<T, W>::array_t &_arr)
      : arr(_arr)
  {
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::pack(const typename pack<T, W>::half_array_t &a,
                                const typename pack<T, W>::half_array_t &b)
  {
    int i = 0;
    for (int j = 0; j < int(a.size()); j++, i++)
      arr[i] = a[j];
    for (int j = 0; j < int(b.size()); j++, i++)
      arr[i] = b[j];
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> &pack<T, W>::operator=(const element_t &v)
  {
    *this = pack<T, W>(v);
    return *this;
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
  TSIMD_INLINE pack<T, W>::operator const typename pack<T, W>::array_t &() const
  {
    return arr;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W>::operator typename pack<T, W>::array_t &()
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
  TSIMD_INLINE typename pack<T, W>::iterator_t pack<T, W>::begin()
  {
#if TSIMD_WIN
    return &arr[0];
#else
    return arr.begin();
#endif
  }

  template <typename T, int W>
  TSIMD_INLINE typename pack<T, W>::iterator_t pack<T, W>::end()
  {
    return begin() + W;
  }

  template <typename T, int W>
  TSIMD_INLINE typename pack<T, W>::const_iterator_t pack<T, W>::begin() const
  {
#if TSIMD_WIN
    return &arr[0];
#else
    return arr.begin();
#endif
  }

  template <typename T, int W>
  TSIMD_INLINE typename pack<T, W>::const_iterator_t pack<T, W>::end() const
  {
    return begin() + W;
  }

  template <typename T, int W>
  TSIMD_INLINE typename pack<T, W>::const_iterator_t pack<T, W>::cbegin() const
  {
    return begin();
  }

  template <typename T, int W>
  TSIMD_INLINE typename pack<T, W>::const_iterator_t pack<T, W>::cend() const
  {
    return end();
  }

#if defined(__AVX512VL__)
  template <>
  TSIMD_INLINE typename pack<bool32_t, 8>::iterator_t
  pack<bool32_t, 8>::begin()
  {
    return bit_iterator((void*)&v, 0);
  }

  template <>
  TSIMD_INLINE typename pack<bool32_t, 8>::iterator_t
  pack<bool32_t, 8>::end()
  {
    return bit_iterator((void*)&v, 8);
  }

  template <>
  TSIMD_INLINE typename pack<bool32_t, 8>::const_iterator_t
  pack<bool32_t, 8>::begin() const
  {
    return bit_iterator((void*)&v, 0);
  }

  template <>
  TSIMD_INLINE typename pack<bool32_t, 8>::const_iterator_t
  pack<bool32_t, 8>::end() const
  {
    return bit_iterator((void*)&v, 8);
  }
#endif

#if defined(__AVX512F__)
  // 8-wide //

  template <>
  TSIMD_INLINE typename pack<bool64_t, 8>::iterator_t
  pack<bool64_t, 8>::begin()
  {
    return bit_iterator((void*)&v, 0);
  }

  template <>
  TSIMD_INLINE typename pack<bool64_t, 8>::iterator_t
  pack<bool64_t, 8>::end()
  {
    return bit_iterator((void*)&v, 8);
  }

  template <>
  TSIMD_INLINE typename pack<bool64_t, 8>::const_iterator_t
  pack<bool64_t, 8>::begin() const
  {
    return bit_iterator((void*)&v, 0);
  }

  template <>
  TSIMD_INLINE typename pack<bool64_t, 8>::const_iterator_t
  pack<bool64_t, 8>::end() const
  {
    return bit_iterator((void*)&v, 8);
  }

  // 16-wide //

  template <>
  TSIMD_INLINE typename pack<bool32_t, 16>::iterator_t
  pack<bool32_t, 16>::begin()
  {
    return bit_iterator((void*)&v, 0);
  }

  template <>
  TSIMD_INLINE typename pack<bool32_t, 16>::iterator_t
  pack<bool32_t, 16>::end()
  {
    return bit_iterator((void*)&v, 16);
  }

  template <>
  TSIMD_INLINE typename pack<bool32_t, 16>::const_iterator_t
  pack<bool32_t, 16>::begin() const
  {
    return bit_iterator((void*)&v, 0);
  }

  template <>
  TSIMD_INLINE typename pack<bool32_t, 16>::const_iterator_t
  pack<bool32_t, 16>::end() const
  {
    return bit_iterator((void*)&v, 16);
  }

  template <>
  TSIMD_INLINE typename pack<bool64_t, 16>::iterator_t
  pack<bool64_t, 16>::begin()
  {
    return bit_iterator((void*)&v, 0);
  }

  template <>
  TSIMD_INLINE typename pack<bool64_t, 16>::iterator_t
  pack<bool64_t, 16>::end()
  {
    return bit_iterator((void*)&v, 16);
  }

  template <>
  TSIMD_INLINE typename pack<bool64_t, 16>::const_iterator_t
  pack<bool64_t, 16>::begin() const
  {
    return bit_iterator((void*)&v, 0);
  }

  template <>
  TSIMD_INLINE typename pack<bool64_t, 16>::const_iterator_t
  pack<bool64_t, 16>::end() const
  {
    return bit_iterator((void*)&v, 16);
  }
#endif

  template <typename T, int W>
  TSIMD_INLINE T pack<T, W>::extract(int i) const
  {
    return (*this)[i];
  }

  template <typename T, int W>
  TSIMD_INLINE void pack<T, W>::insert(T v, int i)
  {
    (*this)[i] = v;
  }

  template <typename T, int W>
  TSIMD_INLINE typename pack<T, W>::const_iterator_deref_t
  pack<T, W>::operator[](int i) const
  {
    return *(begin() + i);
  }

  template <typename T, int W>
  TSIMD_INLINE typename pack<T, W>::iterator_deref_t
  pack<T, W>::operator[](int i)
  {
    return *(begin() + i);
  }

  // pack<> debugging functions ///////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE std::ostream &operator<<(std::ostream &o, const pack<T, W> &p)
  {
    o << "{";

    for (const auto v : p)
      o << " " << v;

    o << " }";

    return o;
  }

  template <typename T, int W>
  TSIMD_INLINE void print(const pack<T, W> &p)
  {
    std::cout << p << std::endl;
  }

  // pack<> free-form insert()/extract() //////////////////////////////////////

  template <typename T, int W, typename = traits::valid_type_for_pack<T>>
  TSIMD_INLINE void insert(pack<T, W> &p, int itemID, T v)
  {
    p.insert(v, itemID);
  }

  template <typename T, int W, typename = traits::valid_type_for_pack<T>>
  TSIMD_INLINE T extract(const pack<T, W> &p, int itemID)
  {
    return p.extract(itemID);
  }

  template <typename T, int W, typename = traits::valid_type_for_pack<T>>
  TSIMD_INLINE void insert(pack<T, W> &p, int itemID, const pack<T, 1> &v)
  {
    p.insert(v[0], itemID);
  }

  // pack<> cast definition ///////////////////////////////////////////////////

  template <typename OTHER_T, typename T, int W>
  TSIMD_INLINE pack<OTHER_T, W> convert_elements_to(const pack<T, W> &from)
  {
    pack<OTHER_T, W> to;

    for (int i = 0; i < W; ++i)
      to[i] = from[i];

    return to;
  }

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> convert_elements_to(const pack<T, W> &from)
  {
    return from;
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
