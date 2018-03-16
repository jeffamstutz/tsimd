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

#include <ostream>

namespace tsimd {

  // 32-bit bool type /////////////////////////////////////////////////////////

  // NOTE: this type is to be used for the per-element 32-bit SIMD mask types
  struct bool32_t
  {
    bool32_t()                 = default;
    bool32_t(const bool32_t &) = default;
    bool32_t(bool32_t &&)      = default;
    bool32_t &operator=(const bool32_t &) = default;
    bool32_t &operator=(bool32_t &&) = default;

    TSIMD_INLINE bool32_t(bool b) noexcept
    {
      // opt: if b is 0/1, the same result can be gotten by 'value = -b'
      value = b ? 0xFFFFFFFF : 0x0;
    }

    TSIMD_INLINE bool32_t &operator=(bool b) noexcept
    {
      // opt: if b is 0/1, the same result can be gotten by 'value = -b'
      value = b ? 0xFFFFFFFF : 0x0;
      return *this;
    }

    TSIMD_INLINE operator bool() const noexcept
    {
      return value == 0xFFFFFFFF;
    }

    uint32_t value;
  };

  // Inlined operators ////////////////////////////////////////////////////////

  TSIMD_INLINE bool32_t operator!(const bool32_t &p1)
  {
    return !p1.value;
  }

  TSIMD_INLINE bool32_t operator|(const bool32_t &p1, const bool32_t &p2)
  {
    return p1.value | p2.value;
  }

  TSIMD_INLINE bool32_t operator&(const bool32_t &p1, const bool32_t &p2)
  {
    return p1.value & p2.value;
  }

  TSIMD_INLINE bool32_t operator^(const bool32_t &p1, const bool32_t &p2)
  {
    return p1.value ^ p2.value;
  }

  TSIMD_INLINE bool32_t operator==(const bool32_t &p1, const bool32_t &p2)
  {
    return p1.value == p2.value;
  }

  TSIMD_INLINE std::ostream &operator<<(std::ostream &o, const bool32_t &v)
  {
    o << static_cast<bool>(v);
    return o;
  }

  // 64-bit bool type /////////////////////////////////////////////////////////

  // NOTE: this type is to be used for the per-element 64-bit SIMD mask types
  struct bool64_t
  {
    bool64_t()                 = default;
    bool64_t(const bool64_t &) = default;
    bool64_t(bool64_t &&)      = default;
    bool64_t &operator=(const bool64_t &) = default;
    bool64_t &operator=(bool64_t &&) = default;

    TSIMD_INLINE bool64_t(bool b) noexcept
    {
      // opt: if b is 0/1, the same result can be gotten by 'value = -b'
      value = b ? 0xFFFFFFFFFFFFFFFFLL : 0x0;
    }

    TSIMD_INLINE bool64_t &operator=(bool b) noexcept
    {
      // opt: if b is 0/1, the same result can be gotten by 'value = -b'
      value = b ? 0xFFFFFFFFFFFFFFFFLL : 0x0;
      return *this;
    }

    TSIMD_INLINE operator bool() const noexcept
    {
      return value == 0xFFFFFFFFFFFFFFFFLL;
    }

    uint64_t value;
  };

  // Inlined operators ////////////////////////////////////////////////////////

  TSIMD_INLINE bool64_t operator!(const bool64_t &p1)
  {
    return !p1.value;
  }

  TSIMD_INLINE bool64_t operator|(const bool64_t &p1, const bool64_t &p2)
  {
    return p1.value | p2.value;
  }

  TSIMD_INLINE bool64_t operator&(const bool64_t &p1, const bool64_t &p2)
  {
    return p1.value & p2.value;
  }

  TSIMD_INLINE bool64_t operator^(const bool64_t &p1, const bool64_t &p2)
  {
    return p1.value ^ p2.value;
  }

  TSIMD_INLINE bool64_t operator==(const bool64_t &p1, const bool64_t &p2)
  {
    return p1.value == p2.value;
  }

  TSIMD_INLINE std::ostream &operator<<(std::ostream &o, const bool64_t &v)
  {
    o << static_cast<bool>(v);
    return o;
  }

}  // namespace tsimd
