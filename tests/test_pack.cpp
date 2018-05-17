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

#include "catch/catch.hpp"

#include "tsimd/tsimd.h"

#include <algorithm>
#include <numeric>
#include <random>

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

// pack<> member functions ////////////////////////////////////////////////////

TEST_CASE("bit_iterator interface", "[bit_iterator]")
{
  size_t bits = 0;

  tsimd::bit_iterator itr(bits, 0);

  REQUIRE(*itr == false);

  *itr = true;
  REQUIRE(*itr == true);

  bits = 3;

  REQUIRE(*itr == true);
  itr++;
  REQUIRE(*itr == true);
  itr++;
  REQUIRE(*itr == false);
  itr--;
  REQUIRE(*itr == true);
}

// static type checking ///////////////////////////////////////////////////////

// element promotion from float to double
using float_to_double_plus   = decltype(tsimd::vfloat() + tsimd::vdouble());
using float_to_double_minus  = decltype(tsimd::vfloat() - tsimd::vdouble());
using float_to_double_times  = decltype(tsimd::vfloat() * tsimd::vdouble());
using float_to_double_divide = decltype(tsimd::vfloat() / tsimd::vdouble());

static_assert(std::is_same<float_to_double_plus, tsimd::vdouble>::value, "");
static_assert(std::is_same<float_to_double_minus, tsimd::vdouble>::value, "");
static_assert(std::is_same<float_to_double_times, tsimd::vdouble>::value, "");
static_assert(std::is_same<float_to_double_divide, tsimd::vdouble>::value, "");

// element promotion from int to float
using int_to_float_plus   = decltype(tsimd::vint() + tsimd::vfloat());
using int_to_float_minus  = decltype(tsimd::vint() - tsimd::vfloat());
using int_to_float_times  = decltype(tsimd::vint() * tsimd::vfloat());
using int_to_float_divide = decltype(tsimd::vint() / tsimd::vfloat());

static_assert(std::is_same<int_to_float_plus, tsimd::vfloat>::value, "");
static_assert(std::is_same<int_to_float_minus, tsimd::vfloat>::value, "");
static_assert(std::is_same<int_to_float_times, tsimd::vfloat>::value, "");
static_assert(std::is_same<int_to_float_divide, tsimd::vfloat>::value, "");

// element promotion from int to llong
using int_to_llong_plus   = decltype(tsimd::vint() + tsimd::vllong());
using int_to_llong_minus  = decltype(tsimd::vint() - tsimd::vllong());
using int_to_llong_times  = decltype(tsimd::vint() * tsimd::vllong());
using int_to_llong_divide = decltype(tsimd::vint() / tsimd::vllong());

static_assert(std::is_same<int_to_llong_plus, tsimd::vllong>::value, "");
static_assert(std::is_same<int_to_llong_minus, tsimd::vllong>::value, "");
static_assert(std::is_same<int_to_llong_times, tsimd::vllong>::value, "");
static_assert(std::is_same<int_to_llong_divide, tsimd::vllong>::value, "");

// pack<> member functions ////////////////////////////////////////////////////

TEST_CASE("cast construction", "[member_functions]")
{
  vfloat v(3.5f);

  REQUIRE(tsimd::all(vint(v) == 3));

  tsimd::vint4 v4(0, 1, 2, 3);
  tsimd::vint4 test4;
  std::iota(test4.begin(), test4.end(), 0);
  REQUIRE(tsimd::all(v4 == test4));

  tsimd::vint8 v8(0, 1, 2, 3, 4, 5, 6, 7);
  tsimd::vint8 test8;
  std::iota(test8.begin(), test8.end(), 0);
  REQUIRE(tsimd::all(v8 == test8));

  tsimd::vint16 v16(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  tsimd::vint16 test16;
  std::iota(test16.begin(), test16.end(), 0);
  REQUIRE(tsimd::all(v16 == test16));
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

TEST_CASE("binary operator&()", "[bitwise_operators]")
{
  vbool m1(true);
  vbool m2(false);

  REQUIRE(tsimd::none(m1 & m2));
}

TEST_CASE("binary operator|()", "[bitwise_operators]")
{
  vbool m1(true);
  vbool m2(false);

  REQUIRE(tsimd::all(m1 | m2));
}

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
  vint v2(3);

  REQUIRE(tsimd::all((v1 ^ v2) == vint(2)));
  REQUIRE(tsimd::all((v1 ^ 3) == vint(2)));
  REQUIRE(tsimd::all((3 ^ v1) == vint(2)));
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

TEST_CASE("ceil()", "[math_functions]")
{
  vfloat v(1.5f);
  REQUIRE(tsimd::all(tsimd::near_equal(tsimd::ceil(v), 2.f)));
}

TEST_CASE("floor()", "[math_functions]")
{
  vfloat v(1.5f);
  REQUIRE(tsimd::all(tsimd::near_equal(tsimd::floor(v), 1.f)));
}

TEST_CASE("min()", "[math_functions]")
{
  vfloat v1(1.f);
  vfloat v2(2.f);
  REQUIRE(tsimd::all(tsimd::near_equal(tsimd::min(v1, v2), 1.f)));

  vint v3(1);
  vint v4(2);
  REQUIRE(tsimd::all(tsimd::min(v3, v4) == 1));
}

TEST_CASE("max()", "[math_functions]")
{
  vfloat v1(1.f);
  vfloat v2(2.f);
  REQUIRE(tsimd::all(tsimd::near_equal(tsimd::max(v1, v2), 2.f)));

  vint v3(1);
  vint v4(2);
  REQUIRE(tsimd::all(tsimd::max(v3, v4) == 2));
}

TEST_CASE("rcp()", "[math_functions]")
{
  vfloat v1(4.f);
  v1 = tsimd::rcp(v1);
  REQUIRE(tsimd::all(tsimd::near_equal(v1, vfloat(0.25f), float_type(1e-3f))));
}

TEST_CASE("rsqrt()", "[math_functions]")
{
  vfloat v1(4.f);
  v1 = tsimd::rsqrt(v1);
  REQUIRE(tsimd::all(tsimd::near_equal(v1, vfloat(0.5f), float_type(1e-3f))));
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

TEST_CASE("log()", "[math_functions]")
{
  vfloat v(1.f);
  REQUIRE(tsimd::all(tsimd::near_equal(tsimd::log(v), log(1.f))));
}

TEST_CASE("exp()", "[math_functions]")
{
  vfloat v(1.f);
  REQUIRE(tsimd::all(tsimd::near_equal(tsimd::exp(v), exp(1.f))));
}

TEST_CASE("pow()", "[math_functions]")
{
  vfloat v(2.f);
  REQUIRE(tsimd::all(tsimd::near_equal(tsimd::pow(v, 2.5f), pow(2.f, 2.5f))));
}

// pack<> algorithms //////////////////////////////////////////////////////////

TEST_CASE("foreach()", "[algorithms]")
{
  vfloat v1(0.f);
  vfloat v2(1.f);

  foreach (v1, [](float_type &l, int) { l = 1; });

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

TEST_CASE("foreach_unique()", "[algorithms]")
{
  int checker = 0;
  vint v(0);

#if TEST_WIDTH > 1
  v[0] = 2;
  v[2] = 3;

  tsimd::foreach_unique(v, [&](vbool, int_type) { checker++; });
  REQUIRE(checker == 3);
#else
  tsimd::foreach_unique(v, [&](vbool, int_type) { checker++; });
  REQUIRE(checker == 1);
#endif
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

  std::fill(m.begin(), m.end(), true);
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

  REQUIRE(tsimd::all(tsimd::near_equal(v1, v2, vfloat::element_t(0.11))));

  v1 = 2.1f;

  REQUIRE(tsimd::none(tsimd::near_equal(v1, v2, vfloat::element_t(0.11))));
}

// pack<> memory operations ///////////////////////////////////////////////////

TEST_CASE("unmasked load()", "[memory_operations]")
{
  alignas(64) std::array<int_type, vint::static_size> values;
  std::fill(values.begin(), values.end(), 5);

  auto v1 = tsimd::load<vint>(values.data());
  REQUIRE(tsimd::all(v1 == 5));
}

TEST_CASE("masked load()", "[memory_operations]")
{
  if (vbool::static_size > 1) {
    alignas(64) std::array<int_type, vint::static_size> values;
    std::fill(values.begin(), values.end(), 5);

    vbool m(true);
    m[2] = false;

    vint v1(0);
    auto v2 = tsimd::load<vint>(values.data(), m);
    tsimd::set_if(v1, v2, m);

    vint expected(5);
    expected[2] = 0;
    REQUIRE(tsimd::all(v1 == expected));
  }
}

TEST_CASE("unmasked gather()", "[memory_operations]")
{
  alignas(64) std::array<int_type, vint::static_size> values;
  std::fill(values.begin(), values.end(), 4);

  vint offset;
  std::iota(offset.begin(), offset.end(), 0);

  auto result = tsimd::gather<vint>(values.data(), offset);

  REQUIRE(tsimd::all(result == 4));
}

TEST_CASE("unmasked store()", "[memory_operations]")
{
  alignas(64) std::array<int_type, vint::static_size> values;

  vint v1(7);

  tsimd::store(v1, values.data());

  std::for_each(
      values.begin(), values.end(), [](int_type v) { REQUIRE(v == 7); });
}

TEST_CASE("unmasked scatter()", "[memory_operations]")
{
  alignas(64) std::array<int_type, vint::static_size> values;

  vint v1(5);

  vint offset;
  std::iota(offset.begin(), offset.end(), 0);

  tsimd::scatter(v1, values.data(), offset);

  std::for_each(
      values.begin(), values.end(), [](int_type v) { REQUIRE(v == 5); });
}

#if !TEST_DOUBLE_PRECISION
TEST_CASE("reverse_bits()", "[memory_operations]")
{
  vint v1(0x01020304);

  v1 = tsimd::reverse_bits(v1);
  REQUIRE(tsimd::all(v1 == 0x04030201));

  v1 = tsimd::reverse_bits(v1);
  REQUIRE(tsimd::all(v1 == 0x01020304));

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib;
  for (auto &x : v1)
    x = distrib(gen);

  const vint orig = v1;
  v1 = reverse_bits(v1);
  v1 = reverse_bits(v1);
  REQUIRE(tsimd::all(v1 == orig));
}
#endif

// random numbers /////////////////////////////////////////////////////////////

TEST_CASE("uniform_random_distribution()", "[random]")
{
  tsimd::default_halton_engine2<vfloat::static_size> rng;
  tsimd::uniform_real_distribution<vfloat> dist(1.f, 2.f);
  vfloat v = dist(rng);

  REQUIRE(tsimd::all(v >= 1.f & v < 2.f));
}

template <int BASE>
inline void precomputed_halton_test()
{
  tsimd::precomputed_halton_engine<256, BASE, vfloat::static_size> rng;
  auto v = tsimd::generate_canonical(rng);

  REQUIRE(tsimd::all(v >= 0.f & v < 1.f));
  REQUIRE(tsimd::any(v != 0.f));
  REQUIRE(tsimd::any(v != 1.f));
}

TEST_CASE("precomputed_halton_engine<base>()", "[random]")
{
  precomputed_halton_test<2>();
  precomputed_halton_test<3>();
  precomputed_halton_test<4>();
  precomputed_halton_test<5>();
  precomputed_halton_test<6>();
  precomputed_halton_test<7>();
  precomputed_halton_test<8>();
  precomputed_halton_test<9>();
  precomputed_halton_test<10>();
}

