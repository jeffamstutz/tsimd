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

#include "catch/catch.hpp"

#include "tsimd/tsimd.h"

#include <algorithm>
#include <numeric>

#ifndef TEST_WIDTH
#define TEST_WIDTH 1
#endif

#ifndef TEST_DOUBLE_PRECISION
#define TEST_DOUBLE_PRECISION 0
#endif

#if TEST_DOUBLE_PRECISION
using bool_type  = tsimd::bool64_t;
using float_type = double;
using int_type   = long long;
#else
using bool_type  = tsimd::bool32_t;
using float_type = float;
using int_type   = int;
#endif

using vbool  = tsimd::pack<bool_type, TEST_WIDTH>;
using vfloat = tsimd::pack<float_type, TEST_WIDTH>;
using vint   = tsimd::pack<int_type, TEST_WIDTH>;

/* TODO: add tests for -->
 *         - operator<<()
 *         - operator>>()
 *         - operator^()
 *         - load()
 *         - store()
 */

// pack<> member functions ////////////////////////////////////////////////////

TEST_CASE("cast construction", "[member_functions]")
{
  vfloat v1(3.5f);

  REQUIRE(tsimd::all(vint(v1) == 3));
}

// pack<> arithmetic operators ////////////////////////////////////////////////

TEST_CASE("binary operator+()", "[arithmetic_operators]")
{
  vfloat v1(1.f), v2(2.f);

  REQUIRE(tsimd::all(tsimd::near_equal(v1 + v2, 3.f)));
  REQUIRE(tsimd::all(tsimd::near_equal(v1 + 2.f, 3.f)));
  REQUIRE(tsimd::all(tsimd::near_equal(2.f + v1, 3.f)));

  // Add checks to make sure we don't promote regular math!
  bool value = std::is_same<decltype(1.f + 1.f), float>::value;
  value |= std::is_same<decltype(1.f + 1.), double>::value;
  value |= std::is_same<decltype(1.f + 1), float>::value;
  REQUIRE(value);
}

TEST_CASE("binary operator+=()", "[arithmetic_operators]")
{
  vfloat v1(1.f), v2(2.f);

  v1 += v2;
  v2 += 1.f;

  REQUIRE(tsimd::all(tsimd::near_equal(v1, 3.f)));
  REQUIRE(tsimd::all(tsimd::near_equal(v2, 3.f)));
}

TEST_CASE("binary operator-()", "[arithmetic_operators]")
{
  vfloat v1(2.f), v2(1.f);

  REQUIRE(tsimd::all(tsimd::near_equal(v1 - v2, 1.f)));
  REQUIRE(tsimd::all(tsimd::near_equal(v1 - 2.f, 0.f)));
  REQUIRE(tsimd::all(tsimd::near_equal(4.f - v1, 2.f)));

  // Add checks to make sure we don't promote regular math!
  bool value = std::is_same<decltype(1.f - 1.f), float>::value;
  value |= std::is_same<decltype(1.f - 1.), double>::value;
  value |= std::is_same<decltype(1.f - 1), float>::value;
  REQUIRE(value);
}

TEST_CASE("binary operator-=()", "[arithmetic_operators]")
{
  vint v1(1), v2(2);

  v1 -= v2;
  v2 -= 1;

  REQUIRE(tsimd::all(v1 == vint(-1)));
  REQUIRE(tsimd::all(v2 == vint(1)));
}

TEST_CASE("binary operator*()", "[arithmetic_operators]")
{
  vfloat v1(2.f), v2(1.f);

  REQUIRE(tsimd::all(tsimd::near_equal(v1 * v2, 2.f)));
  REQUIRE(tsimd::all(tsimd::near_equal(v1 * 2.f, 4.f)));
  REQUIRE(tsimd::all(tsimd::near_equal(2.f * v1, 4.f)));

  // Add checks to make sure we don't promote regular math!
  bool value = std::is_same<decltype(1.f * 1.f), float>::value;
  value |= std::is_same<decltype(1.f * 1.), double>::value;
  value |= std::is_same<decltype(1.f * 1), float>::value;
  REQUIRE(value);
}

TEST_CASE("binary operator*=()", "[arithmetic_operators]")
{
  vint v1(1), v2(2);

  v1 *= v2;
  v2 *= 2;

  REQUIRE(tsimd::all(v1 == vint(2)));
  REQUIRE(tsimd::all(v2 == vint(4)));
}

TEST_CASE("binary operator/()", "[arithmetic_operators]")
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

TEST_CASE("binary operator/=()", "[arithmetic_operators]")
{
  vint v1(8), v2(4);

  v1 /= v2;
  v2 /= 2;

  REQUIRE(tsimd::all(v1 == vint(2)));
  REQUIRE(tsimd::all(v2 == vint(2)));
}

TEST_CASE("binary operator%()", "[arithmetic_operators]")
{
  vint v1(4), v2(3);

  REQUIRE(tsimd::all((v1 % v2) == vint(1)));
  REQUIRE(tsimd::all((v1 % 8) == vint(4)));
  REQUIRE(tsimd::all((8 % v1) == vint(0)));

  // Add checks to make sure we don't promote regular math!
  bool value = std::is_same<decltype(1 % 1), int>::value;
  REQUIRE(value);
}

TEST_CASE("binary operator%=()", "[arithmetic_operators]")
{
  vint v1(5), v2(4);

  v1 %= v2;
  v2 %= 3;

  REQUIRE(tsimd::all(v1 == vint(1)));
  REQUIRE(tsimd::all(v2 == vint(1)));
}

// pack<> bitwise operators ///////////////////////////////////////////////////

TEST_CASE("binary operator<<()", "[bitwise_operators]")
{
  vint v1(1);
  vint v2(1);

  REQUIRE(tsimd::all((v1 << v2) == vint(2)));
  REQUIRE(tsimd::all((v1 << 1) == vint(2)));
  REQUIRE(tsimd::all((1 << v1) == vint(2)));
}

TEST_CASE("binary operator>>()", "[bitwise_operators]")
{
  vint v1(2);
  vint v2(1);

  REQUIRE(tsimd::all((v1 >> v2) == vint(1)));
  REQUIRE(tsimd::all((v1 >> 1) == vint(1)));
  REQUIRE(tsimd::all((4 >> v1) == vint(1)));
}

TEST_CASE("binary operator^()", "[bitwise_operators]")
{
  vint v1(1);
  vint v2(2);

  REQUIRE(tsimd::all((v1 ^ v2) == vint(3)));
  REQUIRE(tsimd::all((v1 ^ 2) == vint(3)));
  REQUIRE(tsimd::all((2 ^ v1) == vint(3)));
}

// pack<> logic operators /////////////////////////////////////////////////////

TEST_CASE("binary operator==()", "[logic_operators]")
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

TEST_CASE("binary operator!=()", "[logic_operators]")
{
  vint v1(1);
  vint v2(2);

  REQUIRE(tsimd::all(v1 != v2));
  REQUIRE(tsimd::all(1 != v2));
  REQUIRE(tsimd::all(v2 != 1));

  v1[0] = 2;

  REQUIRE(!tsimd::all(v1 != v2));
}

TEST_CASE("binary operator<()", "[logic_operators]")
{
  vint v1(1);
  vint v2(2);

  REQUIRE(tsimd::all(v1 < v2));
  REQUIRE(tsimd::all(1 < v2));
  REQUIRE(tsimd::all(v1 < 2));
}

TEST_CASE("binary operator<=()", "[logic_operators]")
{
  vint v1(1);
  vint v2(2);

  v1[0] = 2;

  REQUIRE(tsimd::all(v1 <= v2));
  REQUIRE(tsimd::all(1 <= v2));
  REQUIRE(tsimd::all(v1 <= 2));
}

TEST_CASE("binary operator>()", "[logic_operators]")
{
  vint v1(2);
  vint v2(1);

  REQUIRE(tsimd::all(v1 > v2));
  REQUIRE(tsimd::all(2 > v2));
  REQUIRE(tsimd::all(v1 > 1));
}

TEST_CASE("binary operator>=()", "[logic_operators]")
{
  vint v1(2);
  vint v2(1);

  v2[0] = 0;

  REQUIRE(tsimd::all(v1 >= v2));
  REQUIRE(tsimd::all(1 >= v2));
  REQUIRE(tsimd::all(v1 >= 2));
}

TEST_CASE("binary operator&&()", "[logic_operators]")
{
  vbool m1(true);
  vbool m2(false);

  REQUIRE(tsimd::none(m1 && m2));
}

TEST_CASE("binary operator||()", "[logic_operators]")
{
  vbool m1(true);
  vbool m2(false);

  REQUIRE(tsimd::all(m1 || m2));
}

TEST_CASE("unary operator!()", "[logic_operators]")
{
  vbool v(true);

  REQUIRE(tsimd::all(!v == vbool(false)));
}

TEST_CASE("unary operator-()", "[logic_operators]")
{
  vint v1(2);

  REQUIRE(tsimd::all(-v1 == vint(-2)));
}

// pack<> math functions //////////////////////////////////////////////////////

TEST_CASE("abs()", "[math_functions]")
{
  vint v1(-4);
  REQUIRE(tsimd::all((v1) != vint(4)));
  v1 = tsimd::abs(v1);
  REQUIRE(tsimd::all((v1) == vint(4)));
}

TEST_CASE("sqrt()", "[math_functions]")
{
  vfloat v1(4.f);
  v1 = tsimd::sqrt(v1);
  REQUIRE(tsimd::all(tsimd::near_equal(v1, 2.f)));
}

TEST_CASE("sin()", "[math_functions]")
{
  vfloat v1(4.f);
  v1 = tsimd::sin(v1);
  REQUIRE(tsimd::all(tsimd::near_equal(v1, sin(4.f))));
}

TEST_CASE("cos()", "[math_functions]")
{
  vfloat v1(4.f);
  v1 = tsimd::cos(v1);
  REQUIRE(tsimd::all(tsimd::near_equal(v1, cos(4.f))));
}

TEST_CASE("tan()", "[math_functions]")
{
  vfloat v1(4.f);
  v1 = tsimd::tan(v1);
  REQUIRE(tsimd::all(tsimd::near_equal(v1, tan(4.f))));
}

// pack<> algorithms //////////////////////////////////////////////////////////

TEST_CASE("foreach()", "[algorithms]")
{
  vfloat v1(0.f);
  vfloat v2(1.f);

  foreach (v1, [](float_type &l, int) { l = 1; })
    ;

  REQUIRE(tsimd::all(v1 == v2));
}

TEST_CASE("foreach_active()", "[algorithms]")
{
  if (vbool::static_size > 1) {
    vbool m(false);

    m[0] = true;
    m[2] = true;

    vint v1(0);

    vint expected(0);
    expected[0] = 2;
    expected[2] = 2;

    tsimd::foreach_active(m, v1, [](int_type &v) { v = 2; });

    REQUIRE(tsimd::all(v1 == expected));
  } else {
    vbool mf(false);
    vbool mt(true);

    int checker = 0;

    tsimd::foreach_active(mf, [&](int) { checker++; });
    REQUIRE(checker == 0);
    tsimd::foreach_active(mt, [&](int) { checker++; });
    REQUIRE(checker == 1);
  }
}

TEST_CASE("any()", "[algorithms]")
{
  vbool m(false);
  REQUIRE(!tsimd::any(m));
  m[0] = true;
  REQUIRE(tsimd::any(m));
}

TEST_CASE("none()", "[algorithms]")
{
  vbool m(false);
  REQUIRE(tsimd::none(m));
  m[0] = true;
  REQUIRE(!tsimd::none(m));
}

TEST_CASE("all()", "[algorithms]")
{
  vbool m(false);
  REQUIRE(!tsimd::all(m));

  if (vbool::static_size > 1) {
    m[0] = true;
    REQUIRE(!tsimd::all(m));
  }

  foreach (m, [](bool_type &l, int) { l = true; })
    ;
  REQUIRE(tsimd::all(m));
}

TEST_CASE("select()", "[algorithms]")
{
  if (vbool::static_size > 1) {
    vbool m(false);
    m[0] = true;
    m[2] = true;

    vint v1(0);
    vint v2(2);

    REQUIRE(tsimd::all(v1 != v2));

    auto result = tsimd::select(m, v1, v2);

    vint expected(2);
    expected[0] = 0;
    expected[2] = 0;

    REQUIRE(tsimd::all(result == expected));
    REQUIRE(tsimd::any(v1 != expected));
    REQUIRE(tsimd::any(v2 != expected));
  } else {
    vbool mt(true);
    vbool mf(false);

    vint v1(0);
    vint v2(2);

    auto result_true  = tsimd::select(mt, v1, v2);
    auto result_false = tsimd::select(mf, v1, v2);

    REQUIRE(tsimd::all(result_true == v1));
    REQUIRE(tsimd::all(result_false == v2));
  }
}

TEST_CASE("near_equal()", "[algorithms]")
{
  vfloat v1(1.f);
  vfloat v2(1.01f);

  REQUIRE(tsimd::all(tsimd::near_equal(v1, v2, vfloat::value_t(0.11))));

  v1 = 2.1f;

  REQUIRE(tsimd::none(tsimd::near_equal(v1, v2, vfloat::value_t(0.11))));
}

// pack<> memory operations ///////////////////////////////////////////////////

TEST_CASE("unmasked load()", "[memory_operations]")
{
  TSIMD_ALIGN(32) std::array<int_type, vint::static_size> values;
  std::fill(values.begin(), values.end(), 5);

  auto v1 = tsimd::load<vint>(values.data());
  REQUIRE(tsimd::all(v1 == 5));
}

TEST_CASE("masked load()", "[memory_operations]")
{
  TSIMD_ALIGN(32) std::array<int_type, vint::static_size> values;
  std::fill(values.begin(), values.end(), 5);

  vbool m(true);
  m[2] = false;

  vint v1(0);
  v1 = tsimd::load<vint>(values.data(), m);

  vint expected(5);
  expected[2] = 0;
  REQUIRE(tsimd::all(v1 == expected));
}

TEST_CASE("unmasked gather()", "[memory_operations]")
{
  TSIMD_ALIGN(32) std::array<int_type, vint::static_size> values;
  std::fill(values.begin(), values.end(), 4);

  vint offset;
  std::iota(offset.begin(), offset.end(), 0);

  auto result = tsimd::gather<vint>(values.data(), offset);

  REQUIRE(tsimd::all(result == 4));
}

TEST_CASE("unmasked store()", "[memory_operations]")
{
  TSIMD_ALIGN(32) std::array<int_type, vint::static_size> values;

  vint v1(7);

  tsimd::store(v1, values.data());

  std::for_each(
      values.begin(), values.end(), [](int_type v) { REQUIRE(v == 7); });
}

TEST_CASE("unmasked scatter()", "[memory_operations]")
{
  TSIMD_ALIGN(32) std::array<int_type, vint::static_size> values;

  vint v1(5);

  vint offset;
  std::iota(offset.begin(), offset.end(), 0);

  tsimd::scatter(v1, values.data(), offset);

  std::for_each(
      values.begin(), values.end(), [](int_type v) { REQUIRE(v == 5); });
}