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

#include "config.h"

#include "traits/array_for_pack.h"
#include "traits/bool_type_for.h"
#include "traits/can_convert.h"
#include "traits/cast_simd_type.h"
#include "traits/enable_if_t.h"
#include "traits/half_simd_type.h"
#include "traits/is_bool.h"
#include "traits/is_floating_point.h"
#include "traits/is_mask.h"
#include "traits/is_n_bytes.h"
#include "traits/is_pack_of_width.h"
#include "traits/is_pack.h"
#include "traits/is_same_t.h"
#include "traits/pack_iterator.h"
#include "traits/same_size.h"
#include "traits/simd_type.h"
#include "traits/valid_pack_scalar_operator.h"
#include "traits/valid_type_for_pack_ctor.h"
#include "traits/valid_type_for_pack.h"
