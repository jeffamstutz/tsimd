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

#include "../../pack.h"

#include "../bitwise/and.h"

namespace tsimd {

  // binary operator&() ///////////////////////////////////////////////////////

  // 1-wide //

  template <typename T>
  TSIMD_INLINE pack<T, 1> operator&&(const pack<T, 1> &p1, const pack<T, 1> &p2)
  {
    return pack<T, 1>(p1[0] && p2[0]);
  }

  template <typename T, typename OTHER_T>
  TSIMD_INLINE pack<T, 1> operator&&(const pack<T, 1> &p1, const OTHER_T &v)
  {
    return p1[0] && v;
  }

  template <typename T, typename OTHER_T>
  TSIMD_INLINE pack<T, 1> operator&&(const OTHER_T &v, const pack<T, 1> &p1)
  {
    return v && p1[0];
  }

  // 4-wide //

  // TODO

  // 8-wide //

  template <typename T>
  TSIMD_INLINE pack<T, 8> operator &&(const pack<T, 8>& p1,
                                      const pack<T, 8>& p2)
  {
    return p1 & p2;
  }

  template <typename T>
  TSIMD_INLINE pack<T, 8> operator &&(const pack<T, 8>& p1, float v)
  {
    return p1 & pack<T, 8>(v);
  }

  template <typename T>
  TSIMD_INLINE pack<T, 8> operator &&(float v, const pack<T, 8>& p1)
  {
    return pack<T, 8>(v) & p1;
  }

  // 16-wide //

  // TODO

} // ::tsimd
