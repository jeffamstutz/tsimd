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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "tsimd/tsimd.h"

#include <algorithm>
#include <vector>

using tsimd::vfloat;
using tsimd::vint;
using tsimd::vbool;
using vmask = tsimd::vbool;

using tsimd::vtrue;
using tsimd::vfalse;

/* TODO: add tests for -->
 *         - operator<<()
 *         - operator>>()
 *         - operator^()
 *         - load()
 *         - store()
 */

// pack<> arithmetic operators ////////////////////////////////////////////////

TEST_SUITE_BEGIN("arithmetic operators");

TEST_CASE("binary operator+()")
{
  vfloat v1(1.f), v2(2.f);

  REQUIRE(tsimd::all((v1 + v2) == vfloat(3.f)));
  REQUIRE(tsimd::all((v1 + 2.f) == vfloat(3.f)));
  REQUIRE(tsimd::all((2.f + v1) == vfloat(3.f)));

  // Add checks to make sure we don't promote regular math!
  bool value = std::is_same<decltype(1.f + 1.f), float>::value;
  value |= std::is_same<decltype(1.f + 1.), double>::value;
  value |= std::is_same<decltype(1.f + 1), float>::value;
  REQUIRE(value);
}

TEST_CASE("binary operator+=()")
{
  vfloat v1(1.f), v2(2.f);

  v1 += v2;
  v2 += 1.f;

  REQUIRE(tsimd::all(v1 == vfloat(3.f)));
  REQUIRE(tsimd::all(v2 == vfloat(3.f)));
}

TEST_CASE("binary operator-()")
{
  vfloat v1(2.f), v2(1.f);

  REQUIRE(tsimd::all((v1 - v2) == vfloat(1.f)));
  REQUIRE(tsimd::all((v1 - 2.f) == vfloat(0.f)));
  REQUIRE(tsimd::all((4.f - v1) == vfloat(2.f)));

  // Add checks to make sure we don't promote regular math!
  bool value = std::is_same<decltype(1.f - 1.f), float>::value;
  value |= std::is_same<decltype(1.f - 1.), double>::value;
  value |= std::is_same<decltype(1.f - 1), float>::value;
  REQUIRE(value);
}

TEST_CASE("binary operator-=()")
{
  vint v1(1), v2(2);

  v1 -= v2;
  v2 -= 1;

  REQUIRE(tsimd::all(v1 == vint(-1)));
  REQUIRE(tsimd::all(v2 == vint(1)));
}

TEST_CASE("binary operator*()")
{
  vfloat v1(2.f), v2(1.f);

  REQUIRE(tsimd::all((v1 * v2) == vfloat(2.f)));
  REQUIRE(tsimd::all((v1 * 2.f) == vfloat(4.f)));
  REQUIRE(tsimd::all((2.f * v1) == vfloat(4.f)));

  // Add checks to make sure we don't promote regular math!
  bool value = std::is_same<decltype(1.f * 1.f), float>::value;
  value |= std::is_same<decltype(1.f * 1.), double>::value;
  value |= std::is_same<decltype(1.f * 1), float>::value;
  REQUIRE(value);
}

TEST_CASE("binary operator*=()")
{
  vint v1(1), v2(2);

  v1 *= v2;
  v2 *= 2;

  REQUIRE(tsimd::all(v1 == vint(2)));
  REQUIRE(tsimd::all(v2 == vint(4)));
}

TEST_CASE("binary operator/()")
{
  vint v1(4), v2(2);

  REQUIRE(tsimd::all((v1 / v2) == vint(2)));
  REQUIRE(tsimd::all((v1 / 2) == vint(2)));
  REQUIRE(tsimd::all((8 / v1) == vint(2)));

  // Add checks to make sure we don't promote regular math!
  bool value = std::is_same<decltype(1.f / 1.f), float>::value;
  value |= std::is_same<decltype(1.f / 1.), double>::value;
  value |= std::is_same<decltype(1.f / 1), float>::value;
  REQUIRE(value);
}

TEST_CASE("binary operator/=()")
{
  vint v1(8), v2(4);

  v1 /= v2;
  v2 /= 2;

  REQUIRE(tsimd::all(v1 == vint(2)));
  REQUIRE(tsimd::all(v2 == vint(2)));
}

TEST_CASE("binary operator%()")
{
  vint v1(4), v2(3);

  REQUIRE(tsimd::all((v1 % v2) == vint(1)));
  REQUIRE(tsimd::all((v1 % 8) == vint(4)));
  REQUIRE(tsimd::all((8 % v1) == vint(0)));

  // Add checks to make sure we don't promote regular math!
  bool value = std::is_same<decltype(1 % 1), int>::value;
  REQUIRE(value);
}

TEST_CASE("binary operator%=()")
{
  vint v1(5), v2(4);

  v1 %= v2;
  v2 %= 3;

  REQUIRE(tsimd::all(v1 == vint(1)));
  REQUIRE(tsimd::all(v2 == vint(1)));
}

TEST_SUITE_END();

// pack<> bitwise operators ///////////////////////////////////////////////////

TEST_SUITE_BEGIN("bitwise operators");

TEST_CASE("binary operator<<()")
{
  vint v1(1);
  vint v2(1);

  REQUIRE(tsimd::all((v1 << v2) == vint(2)));
  REQUIRE(tsimd::all((v1 << 1) == vint(2)));
  REQUIRE(tsimd::all((1 << v1) == vint(2)));
}

TEST_CASE("binary operator>>()")
{
  vint v1(2);
  vint v2(1);

  REQUIRE(tsimd::all((v1 >> v2) == vint(1)));
  REQUIRE(tsimd::all((v1 >> 1) == vint(1)));
  REQUIRE(tsimd::all((4 >> v1) == vint(1)));
}

TEST_CASE("binary operator^()")
{
  vint v1(1);
  vint v2(2);

  REQUIRE(tsimd::all((v1 ^ v2) == vint(3)));
  REQUIRE(tsimd::all((v1 ^ 2) == vint(3)));
  REQUIRE(tsimd::all((2 ^ v1) == vint(3)));
}

TEST_SUITE_END();

// pack<> logic operators /////////////////////////////////////////////////////

TEST_SUITE_BEGIN("logic operators");

TEST_CASE("binary operator==()")
{
  vint v1(1);
  vint v2(1);

  REQUIRE(tsimd::all(v1 == v2));
  REQUIRE(tsimd::all(1 == v1));
  REQUIRE(tsimd::all(v1 == 1));

  v1[0] = 2;

  REQUIRE(!tsimd::all(v1 == v2));
  REQUIRE(!tsimd::all(1 == v1));
  REQUIRE(!tsimd::all(v1 == 1));
}

TEST_CASE("binary operator==()")
{
  vint v1(1);
  vint v2(2);

  REQUIRE(tsimd::all(v1 != v2));
  REQUIRE(tsimd::all(1 != v2));
  REQUIRE(tsimd::all(v2 != 1));

  v1[0] = 2;

  REQUIRE(!tsimd::all(v1 != v2));
}

TEST_CASE("binary operator<()")
{
  vint v1(1);
  vint v2(2);

  REQUIRE(tsimd::all(v1 < v2));
  REQUIRE(tsimd::all(1 < v2));
  REQUIRE(tsimd::all(v1 < 2));
}

TEST_CASE("binary operator<=()")
{
  vint v1(1);
  vint v2(2);

  v1[0] = 2;

  REQUIRE(tsimd::all(v1 <= v2));
  REQUIRE(tsimd::all(1 <= v2));
  REQUIRE(tsimd::all(v1 <= 2));
}

TEST_CASE("binary operator>()")
{
  vint v1(2);
  vint v2(1);

  REQUIRE(tsimd::all(v1 > v2));
  REQUIRE(tsimd::all(2 > v2));
  REQUIRE(tsimd::all(v1 > 1));
}

TEST_CASE("binary operator>=()")
{
  vint v1(2);
  vint v2(1);

  v2[0] = 0;

  REQUIRE(tsimd::all(v1 >= v2));
  REQUIRE(tsimd::all(1 >= v2));
  REQUIRE(tsimd::all(v1 >= 2));
}

TEST_CASE("binary operator&&()")
{
  vmask m1(vtrue);
  vmask m2(vfalse);

  REQUIRE(tsimd::none(m1 && m2));
}

TEST_CASE("binary operator||()")
{
  vmask m1(vtrue);
  vmask m2(vfalse);

  REQUIRE(tsimd::all(m1 || m2));
}

TEST_CASE("unary operator!()")
{
  vmask v(vtrue);

  REQUIRE(tsimd::all(!v == vmask(vfalse)));
}

TEST_CASE("unary operator-()")
{
  vint v1(2);

  REQUIRE(tsimd::all(-v1 == vint(-2)));
}

TEST_SUITE_END();

// pack<> math functions //////////////////////////////////////////////////////

TEST_SUITE_BEGIN("math functions");

TEST_CASE("abs()")
{
  vint v1(-4);
  REQUIRE(tsimd::all((v1) != vint(4)));
  v1 = tsimd::abs(v1);
  REQUIRE(tsimd::all((v1) == vint(4)));
}

TEST_CASE("sqrt()")
{
  vfloat v1(4.f);
  v1 = tsimd::sqrt(v1);
  REQUIRE(tsimd::all((v1) == vfloat(2.f)));
}

TEST_CASE("sin()")
{
  vfloat v1(4.f);
  v1 = tsimd::sin(v1);
  REQUIRE(tsimd::all((v1) == vfloat(sin(4.f))));
}

TEST_CASE("cos()")
{
  vfloat v1(4.f);
  v1 = tsimd::cos(v1);
  REQUIRE(tsimd::all((v1) == vfloat(cos(4.f))));
}

TEST_CASE("tan()")
{
  vfloat v1(4.f);
  v1 = tsimd::tan(v1);
  REQUIRE(tsimd::all((v1) == vfloat(tan(4.f))));
}

TEST_SUITE_END();

// pack<> algorithms //////////////////////////////////////////////////////////

TEST_SUITE_BEGIN("algorithms");

TEST_CASE("foreach()")
{
  vfloat v1(0.f);
  vfloat v2(1.f);

  foreach (v1, [](float &l, int) { l = 1; })
    ;

  REQUIRE(tsimd::all(v1 == v2));
}

#if 0  // NOTE: currently crashing on IVB for no obvious reason...
TEST_CASE("foreach_active()")
{
  vbool m(vfalse);

  m[0] = vtrue;
  m[2] = vtrue;

  vint v1(0);

  vint expected(0);
  expected[0] = 2;
  expected[2] = 2;

  tsimd::foreach_active(m, v1, [](int &v){ v = 2; });

  REQUIRE(tsimd::all(v1 == expected));
}
#endif

TEST_CASE("any()")
{
  vmask m(vfalse);
  REQUIRE(!tsimd::any(m));
  m[0] = vtrue;
  REQUIRE(tsimd::any(m));
}

TEST_CASE("none()")
{
  vmask m(vfalse);
  REQUIRE(tsimd::none(m));
  m[0] = vtrue;
  REQUIRE(!tsimd::none(m));
}

TEST_CASE("all()")
{
  vmask m(vfalse);
  REQUIRE(!tsimd::all(m));
  m[0] = vtrue;
  REQUIRE(!tsimd::all(m));
  foreach (m, [](float &l, int) { l = vtrue; })
    ;
  REQUIRE(tsimd::all(m));
}

#if 0  // NOTE: currently crashing on IVB for no obvious reason...
TEST_CASE("select()")
{
  vbool m(vfalse);
  m[0] = vtrue;
  m[2] = vtrue;

  vint v1(0);
  vint v2(2);

  REQUIRE(tsimd::all(v1 != v2));

  auto result = tsimd::select(m, v1, v2);

  vint expected(2);
  expected[0] = 0;
  expected[2] = 0;

  REQUIRE(tsimd::all(result == expected));
  REQUIRE(tsimd::any(v1     != expected));
  REQUIRE(tsimd::any(v2     != expected));
}
#endif

TEST_SUITE_END();

// pack<> memory operations ///////////////////////////////////////////////////

TEST_SUITE_BEGIN("memory operations");

TEST_CASE("unmasked load()")
{
  std::vector<int> values(DEFAULT_WIDTH);
  std::fill(values.begin(), values.end(), 5);

  auto v1 = tsimd::load<vint>(values.data());
  REQUIRE(tsimd::all(v1 == 5));
}

#if 0  // NOTE: currently crashing on IVB for no obvious reason...
TEST_CASE("masked load()")
{
  std::vector<int> values(DEFAULT_WIDTH);
  std::fill(values.begin(), values.end(), 5);

  vbool m(vfalse);
  m[2] = vtrue;

  vint v1(0);
  v1 = tsimd::load<vint>(values.data(), m);

  vint expected(5);
  expected[2] = 0;
  REQUIRE(tsimd::all(v1 == expected));
}
#endif

TEST_CASE("unmasked gather()")
{
  std::vector<int> values(DEFAULT_WIDTH);
  std::fill(values.begin(), values.end(), 4);

  vint offset;
  for (int i = 0; i < DEFAULT_WIDTH; ++i)
    offset[i] = i;

  auto result = tsimd::gather<vint>(values.data(), offset);

  REQUIRE(tsimd::all(result == 4));
}

TEST_CASE("unmasked store()")
{
  std::vector<int> values(DEFAULT_WIDTH);

  vint v1(7);

  tsimd::store(v1, values.data());

  std::for_each(values.begin(), values.end(), [](int v) { REQUIRE(v == 7); });
}

TEST_CASE("unmasked scatter()")
{
  std::vector<int> values(DEFAULT_WIDTH);

  vint v1(5);

  vint offset;
  for (int i = 0; i < DEFAULT_WIDTH; ++i)
    offset[i] = i;

  tsimd::scatter(v1, values.data(), offset);

  std::for_each(values.begin(), values.end(), [](int v) { REQUIRE(v == 5); });
}

TEST_SUITE_END();