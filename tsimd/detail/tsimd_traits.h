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

#include "bool_t.h"
#include "config.h"

namespace tsimd {
  namespace traits {

    // C++14 traits for C++11 /////////////////////////////////////////////////

    template <bool B, class T = void>
    using enable_if_t = typename std::enable_if<B, T>::type;

    // If a single type is convertable to another /////////////////////////////

    template <typename FROM, typename TO>
    using can_convert = enable_if_t<std::is_convertible<TO, FROM>::value>;

    // Is a pack<> with given width ///////////////////////////////////////////

    // TODO: verify if it is indeed a pack! this trait fails if given anything
    //       other than T = pack<something, something>!

    template <typename T, int W>
    struct is_pack_of_width
    {
      static const bool value = (T::static_size == W);
    };

    template <typename T, int W>
    using is_pack_of_width_t = enable_if_t<is_pack_of_width<T, W>::value>;

    // If a given T is a valid type for use in a pack<> ///////////////////////

    template <typename T>
    struct valid_type_for_pack
    {
      static const bool value =
          std::is_same<T, float>::value || std::is_same<T, int>::value ||
          std::is_same<T, bool32_t>::value || std::is_same<T, double>::value ||
          std::is_same<T, long long>::value || std::is_same<T, bool64_t>::value;
    };

    template <typename T>
    using valid_type_for_pack_t = enable_if_t<valid_type_for_pack<T>::value>;

    // If given type is a bool32_t or bool64_t ////////////////////////////////

    template <typename T>
    struct is_bool
    {
      static const bool value =
          std::is_same<T, bool32_t>::value || std::is_same<T, bool64_t>::value;
    };

    template <typename T>
    using is_bool_t = enable_if_t<is_bool<T>::value>;

    // If given pack is a mask (vbool) or not /////////////////////////////////

    template <typename MASK_T>
    struct is_mask
    {
      using value_t           = typename MASK_T::value_t;
      static const bool value = std::is_same<value_t, bool32_t>::value ||
                                std::is_same<value_t, bool64_t>::value;
    };

    template <typename MASK_T>
    using is_mask_t = enable_if_t<is_mask<MASK_T>::value>;

    // Provide intrinsic type given a SIMD width //////////////////////////////

    template <typename T, int W>
    struct simd_undefined_type
    {
    };

    template <typename T, int W>
    struct simd_type
    {
      using type = simd_undefined_type<T, W>;
    };

    // bool types all widths //

    template <int W>
    struct simd_type<bool32_t, W>
    {
      using type = typename simd_type<float, W>::type;
    };

    template <int W>
    struct simd_type<bool64_t, W>
    {
      using type = typename simd_type<double, W>::type;
    };

    // 1-wide //

    template <>
    struct simd_type<float, 1>
    {
      using type = float;
    };

    template <>
    struct simd_type<int, 1>
    {
      using type = int;
    };

    template <>
    struct simd_type<double, 1>
    {
      using type = double;
    };

    template <>
    struct simd_type<long long, 1>
    {
      using type = long long;
    };

    // 4-wide //

#if defined(__SSE__)
    template <>
    struct simd_type<float, 4>
    {
      using type = __m128;
    };

    template <>
    struct simd_type<int, 4>
    {
      using type = __m128i;
    };
#endif

#if defined(__AVX2__) || defined(__AVX__)
    template <>
    struct simd_type<double, 4>
    {
      using type = __m256d;
    };
#endif

#if defined(__AVX2__)
    template <>
    struct simd_type<long long, 4>
    {
      using type = __m256i;
    };
#endif

    // 8-wide //

#if defined(__AVX2__) || defined(__AVX__)
    template <>
    struct simd_type<float, 8>
    {
      using type = __m256;
    };

    template <>
    struct simd_type<int, 8>
    {
      using type = __m256i;
    };
#endif

    // 16-wide //

#if defined(__AVX512F__)
    template <>
    struct simd_type<float, 16>
    {
      using type = __m512;
    };

    template <>
    struct simd_type<int, 16>
    {
      using type = __m512i;
    };

    template <>
    struct simd_type<bool32_t, 16>
    {
      using type = __mmask32;
    };
#endif

    // Provide a cast (float|int) intrinsic type given a SIMD width ///////////

    template <typename T>
    struct cast_simd_undefined_type
    {
    };

    template <typename T, int W>
    struct cast_simd_type
    {
      using type = cast_simd_undefined_type<T>;
    };

    // 1-wide //

    template <>
    struct cast_simd_type<float, 1>
    {
      using type = simd_type<int, 1>::type;
    };

    template <>
    struct cast_simd_type<int, 1>
    {
      using type = simd_type<float, 1>::type;
    };

    template <>
    struct cast_simd_type<double, 1>
    {
      using type = simd_type<long long, 1>::type;
    };

    template <>
    struct cast_simd_type<long long, 1>
    {
      using type = simd_type<double, 1>::type;
    };

    // 4-wide //

    template <>
    struct cast_simd_type<float, 4>
    {
      using type = simd_type<int, 4>::type;
    };

    template <>
    struct cast_simd_type<int, 4>
    {
      using type = simd_type<float, 4>::type;
    };

    template <>
    struct cast_simd_type<bool32_t, 4>
    {
      using type = simd_type<int, 4>::type;
    };

    // 8-wide //

    template <>
    struct cast_simd_type<float, 8>
    {
      using type = simd_type<int, 8>::type;
    };

    template <>
    struct cast_simd_type<int, 8>
    {
      using type = simd_type<float, 8>::type;
    };

    template <>
    struct cast_simd_type<bool32_t, 8>
    {
      using type = simd_type<int, 8>::type;
    };

    // 16-wide //

    template <>
    struct cast_simd_type<float, 16>
    {
      using type = simd_type<int, 16>::type;
    };

    template <>
    struct cast_simd_type<int, 16>
    {
      using type = simd_type<float, 16>::type;
    };

    // Provide intrinsic type half the size of given width ////////////////////

    template <typename T, int W>
    struct half_simd_type
    {
      using type = std::array<T, W / 2>;
    };

    // 1-wide //

    template <typename T>
    struct half_simd_type<T, 1>
    {
      using type = std::array<T, 1>;
    };

    // 8-wide //

#if defined(__SSE__)
    template <>
    struct half_simd_type<float, 8>
    {
      using type = simd_type<float, 4>::type;
    };

    template <>
    struct half_simd_type<int, 8>
    {
      using type = simd_type<int, 4>::type;
    };

    template <>
    struct half_simd_type<bool32_t, 8>
    {
      using type = half_simd_type<float, 8>::type;
    };
#endif

    // 16-wide //

#if defined(__AVX2__) || defined(__AVX__)
    template <>
    struct half_simd_type<float, 16>
    {
      using type = simd_type<float, 8>::type;
    };

    template <>
    struct half_simd_type<bool32_t, 16>
    {
      using type = half_simd_type<float, 16>::type;
    };
#endif

#if defined(__AVX__)
    template <>
    struct half_simd_type<int, 16>
    {
      using type = simd_type<int, 8>::type;
    };
#endif

    // Provide half_simd_type is a std::array<T, W/2> /////////////////////////

    template <typename T, int W>
    struct half_simd_is_array
    {
      static const bool value =
          std::is_same<typename half_simd_type<T, W>::type,
                       std::array<T, W / 2>>::value;
    };

    template <typename T>
    struct half_simd_is_array<T, 1>
    {
      static const bool value =
          std::is_same<typename half_simd_type<T, 1>::type,
                       std::array<T, 1>>::value;
    };

    // Bool type for given primitive type /////////////////////////////////////

    template <typename T>
    struct bool_undefined_type
    {
    };

    template <typename T>
    struct bool_type_for
    {
      using type = bool_undefined_type<T>;
    };

    // 32-bit //

    template <>
    struct bool_type_for<float>
    {
      using type = bool32_t;
    };

    template <>
    struct bool_type_for<int>
    {
      using type = bool32_t;
    };

    template <>
    struct bool_type_for<bool32_t>
    {
      using type = bool32_t;
    };

    // 64-bit //

    template <>
    struct bool_type_for<double>
    {
      using type = bool64_t;
    };

    template <>
    struct bool_type_for<long long>
    {
      using type = bool64_t;
    };

    template <>
    struct bool_type_for<bool64_t>
    {
      using type = bool64_t;
    };

  }  // namespace traits
}  // namespace tsimd