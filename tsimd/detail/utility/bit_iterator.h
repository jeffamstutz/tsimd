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

#include "bit_reference.h"

namespace tsimd {

  struct bit_iterator
  {
    bit_iterator() = default;
    bit_iterator(size_t &src, int which) : bit_iterator(&src, which) {}
    bit_iterator(size_t *src, int which) : ref(src, which) {}
    bit_iterator(void *src, int which) : ref((size_t*)src, which) {}

    bit_iterator(const bit_iterator &) = default;

    bit_reference operator*() const;

    bit_iterator operator++();
    bit_iterator &operator++(int);

    bit_iterator operator--();
    bit_iterator &operator--(int);

    bit_iterator &operator+(const bit_iterator &other);
    bit_iterator &operator-(const bit_iterator &other);

    bit_iterator &operator+(int other);
    bit_iterator &operator-(int other);

    bool operator==(const bit_iterator &other) const;
    bool operator!=(const bit_iterator &other) const;

  private:

    // Data //

    bit_reference ref;
  };

  // Inlined members //////////////////////////////////////////////////////////

  TSIMD_INLINE bit_reference bit_iterator::operator*() const
  {
    return ref;
  }

  TSIMD_INLINE bit_iterator bit_iterator::operator++()
  {
    auto copy = *this;
    ref.lane++;
    return copy;
  }

  TSIMD_INLINE bit_iterator &bit_iterator::operator++(int)
  {
    ref.lane++;
    return *this;
  }

  TSIMD_INLINE bit_iterator bit_iterator::operator--()
  {
    auto copy = *this;
    ref.lane--;
    return copy;
  }

  TSIMD_INLINE bit_iterator &bit_iterator::operator--(int)
  {
    ref.lane--;
    return *this;
  }

  TSIMD_INLINE bit_iterator &bit_iterator::operator+(const bit_iterator &other)
  {
    ref.storage += other.ref.current_offset();
    return *this;
  }

  TSIMD_INLINE bit_iterator &bit_iterator::operator-(const bit_iterator &other)
  {
    ref.storage -= other.ref.current_offset();
    return *this;
  }

  TSIMD_INLINE bit_iterator &bit_iterator::operator+(int offset)
  {
    ref.lane += offset;
    return *this;
  }

  TSIMD_INLINE bit_iterator &bit_iterator::operator-(int offset)
  {
    ref.lane -= offset;
    return *this;
  }

  TSIMD_INLINE bool bit_iterator::operator==(const bit_iterator &other) const
  {
    return ref == other.ref;
  }

  TSIMD_INLINE bool bit_iterator::operator!=(const bit_iterator &other) const
  {
    return !(*this == other);
  }

} // namespace tsimd