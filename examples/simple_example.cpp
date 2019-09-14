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

#include <iostream>
#include <numeric>
#include <vector>

#include "tsimd/tsimd.h"

using namespace tsimd;

int main()
{
  std::cout << "test foreach() and construction" << std::endl;

  {
    vfloat v1(2.f);

    print(v1);
  }

  std::cout << std::endl;

  std::cout << "test operator+()" << std::endl;

  {
    vfloat v1(1.f), v2(2.f);

    std::cout << "#1: " << all((v1 + v2) == vfloat(3.f)) << std::endl;
    std::cout << "#2: " << all((v1 + 2.f) == vfloat(3.f)) << std::endl;
    std::cout << "#3: " << all((2.f + v1) == vfloat(3.f)) << std::endl;
  }

  std::cout << std::endl;

  std::cout << "test operator|(), operator&(), and operator^() on floats"
            << std::endl;

  {
    vdouble v1(1.f), v2(-1.f);
    v2[1] = 2.f;

    std::cout << "#1: " << (v1 | v2) << std::endl;
    std::cout << "#2: " << (v1 & v2) << std::endl;
    std::cout << "#3: " << (v1 ^ v2) << std::endl;
  }

  std::cout << std::endl;

  std::cout << "test sin()/cos()/tan()" << std::endl;

  {
    float value = M_PI;
    vfloat v(value);

    std::cout << "value == " << value << std::endl;
    std::cout << "sin(value) == " << std::sin(value) << std::endl;
    std::cout << "cos(value) == " << std::cos(value) << std::endl;
    std::cout << "tan(value) == " << std::tan(value) << std::endl;
    std::cout << "exp(value) == " << std::exp(value) << std::endl;

    std::cout << "v = vfloat(value)" << std::endl;
    std::cout << "sin(v) == " << sin(v) << std::endl;
    std::cout << "cos(v) == " << cos(v) << std::endl;
    std::cout << "tan(v) == " << tan(v) << std::endl;
    std::cout << "exp(v) == " << exp(v) << std::endl;
  }

  std::cout << std::endl;

  std::cout << "test operator*=()" << std::endl;

  {
    vint v1(1), v2(2);

    std::cout << "before..." << std::endl;
    std::cout << "#1: " << v1 << std::endl;
    std::cout << "#2: " << v2 << std::endl;

    v1 *= v2;
    v2 *= 2;

    std::cout << "after..." << std::endl;
    std::cout << "#1: " << v1 << std::endl;
    std::cout << "#2: " << v2 << std::endl;
  }

  std::cout << std::endl;

  std::cout << "test any()" << std::endl;

  {
    vboolf m(false);

    print(m);

    std::cout << "#1: " << !tsimd::any(m) << std::endl;
    m[0] = true;
    std::cout << "#2: " << tsimd::any(m) << std::endl;
  }

  std::cout << std::endl;

  std::cout << "test operator==()" << std::endl;

  {
    vint v1(1);
    vint v2(2);

    auto b1 = v1 != v2;
    print(b1);

    std::cout << "#1: " << tsimd::all(v1 != v2) << std::endl;
    std::cout << "#2: " << tsimd::all(1 != v2) << std::endl;
    std::cout << "#3: " << tsimd::all(v2 != 1) << std::endl;

    v1[0] = 2;

    std::cout << "#4: " << !tsimd::all(v1 != v2) << std::endl;
  }

  std::cout << std::endl;

  std::cout << "test std::iota(vec)" << std::endl;

  {
    vint v1(0);
    std::iota(v1.begin(), v1.end(), 0);
    print(v1);
  }

  std::cout << std::endl;

  std::cout << "test tsimd::lane_index<int>()" << std::endl;

  {
    print(lane_index<vint>());
  }

  std::cout << std::endl;

  std::cout << "test foreach_active()" << std::endl;

  {
    vboolf m(false);

    m[0] = true;
    m[2] = true;

    vint v(0);

    vint expected(0);
    expected[0] = 2;
    expected[2] = 2;

    tsimd::foreach_active(m, v, [](int &v) { v = 2; });

    print(m);
    print(v);
    print(expected);
  }

  std::cout << std::endl;

  std::cout << "test masked load()" << std::endl;

  {
    std::vector<int> values(vint::static_size);
    std::fill(values.begin(), values.end(), 5);

    vboolf m(false);
    m[2] = true;

    vint v1(0);
    v1 = tsimd::load<vint>(values.data(), m);

    print(v1);
  }

  return 0;
}
