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

#include "bool_t.h"

namespace tsimd {

  struct bit_reference
  {
    bit_reference() = default;
    bit_reference(size_t &src, int which) : bit_reference(&src, which) {}
    bit_reference(size_t *src, int which) : lane(which), storage(src) {}

    bit_reference(const bit_reference &) = default;

    int current_offset() const;

    operator bool() const;
    explicit operator bool32_t() const;
    explicit operator bool64_t() const;

    explicit operator size_t() const;

    bit_reference &operator=(bool bit_value);

    bool operator==(const bit_reference &other) const;
    bool operator!=(const bit_reference &other) const;

  private:

    friend struct bit_iterator;

    // Data //

    int lane {0};
    size_t *storage {nullptr};
  };

  // Inlined members //////////////////////////////////////////////////////////

  TSIMD_INLINE int bit_reference::current_offset() const
  {
    return lane;
  }

  TSIMD_INLINE bit_reference::operator bool() const
  {
    auto &val = *storage;
    return (val >> lane) & 0x1;
  }

  TSIMD_INLINE bit_reference::operator bool32_t() const
  {
    return bool32_t(static_cast<bool>(*this));
  }

  TSIMD_INLINE bit_reference::operator bool64_t() const
  {
    return bool64_t(static_cast<bool>(*this));
  }

  TSIMD_INLINE bit_reference::operator size_t() const
  {
    return *storage;
  }

  TSIMD_INLINE bit_reference &bit_reference::operator=(bool bit_value)
  {
    auto &val = *storage;
    if (bit_value)
      val |= (1u << lane) & 0xFFFFFFFFFFFFFFFFLL;
    else
      val &= (1u << lane) ^ 0xFFFFFFFFFFFFFFFFLL;
    return *this;
  }

  TSIMD_INLINE bool bit_reference::operator==(const bit_reference &other) const
  {
    return (lane == other.lane) && (storage == other.storage);
  }

  TSIMD_INLINE bool bit_reference::operator!=(const bit_reference &other) const
  {
    return !(*this == other);
  }

} // namespace tsimd