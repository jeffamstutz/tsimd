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

#include "enable_if_t.h"
#include "is_pack.h"

namespace tsimd {
  namespace traits {

    // If a given T is a valid type for use in a pack<> ///////////////////////

    template <typename PACK_ELEMENT_T, typename TYPE_IN_QUESTION>
    struct valid_type_for_pack_ctor
    {
      static const bool value =
          std::is_constructible<PACK_ELEMENT_T, TYPE_IN_QUESTION>::value &&
          !std::is_same<TYPE_IN_QUESTION, PACK_ELEMENT_T>::value &&
          !is_pack<TYPE_IN_QUESTION>::value;
    };

    template <typename PACK_ELEMENT_T, typename TYPE_IN_QUESTION>
    using valid_type_for_pack_ctor_t =
        enable_if_t<
          valid_type_for_pack_ctor<PACK_ELEMENT_T, TYPE_IN_QUESTION>::value
        >;

  }  // namespace traits
}  // namespace tsimd