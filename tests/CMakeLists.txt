## ========================================================================== ##
## The MIT License (MIT)                                                      ##
##                                                                            ##
## Copyright (c) 2017 Jefferson Amstutz                                       ##
##                                                                            ##
## Permission is hereby granted, free of charge, to any person obtaining a    ##
## copy of this software and associated documentation files (the "Software"), ##
## to deal in the Software without restriction, including without limitation  ##
## the rights to use, copy, modify, merge, publish, distribute, sublicense,   ##
## and/or sell copies of the Software, and to permit persons to whom the      ##
## Software is furnished to do so, subject to the following conditions:       ##
##                                                                            ##
## The above copyright notice and this permission notice shall be included in ##
## in all copies or substantial portions of the Software.                     ##
##                                                                            ##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR ##
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   ##
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    ##
## THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER ##
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    ##
## FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        ##
## DEALINGS IN THE SOFTWARE.                                                  ##
## ========================================================================== ##

## 1-wide ##

add_executable(test_pack32x1
  doctest/doctest.h
  test_pack.cpp
)

target_compile_definitions(test_pack32x1 PRIVATE -DTEST_WIDTH=1)

set(TEST_EXE1 ${EXECUTABLE_OUTPUT_PATH}/test_pack32x1)

add_test(arithmetic_operators32x1
         ${TEST_EXE1} "--test-suite=\"arithmetic operators\"")

add_test(bitwise_operators32x1
         ${TEST_EXE1} "--test-suite=\"bitwise operators\"")

add_test(logic_operators32x1
         ${TEST_EXE1} "--test-suite=\"logic operators\"")

add_test(math_functions32x1
         ${TEST_EXE1} "--test-suite=\"math functions\"")

add_test(algorithms32x1
         ${TEST_EXE1} "--test-suite=\"algorithms\"")

## 8-wide ##

add_executable(test_pack32x8
  doctest/doctest.h
  test_pack.cpp
)

target_compile_definitions(test_pack32x8 PRIVATE -DTEST_WIDTH=8)

set(TEST_EXE8 ${EXECUTABLE_OUTPUT_PATH}/test_pack32x8)

add_test(arithmetic_operators32x8
         ${TEST_EXE8} "--test-suite=\"arithmetic operators\"")

add_test(bitwise_operators32x8
         ${TEST_EXE8} "--test-suite=\"bitwise operators\"")

add_test(logic_operators32x8
         ${TEST_EXE8} "--test-suite=\"logic operators\"")

add_test(math_functions32x8
         ${TEST_EXE8} "--test-suite=\"math functions\"")

add_test(algorithms32x8
         ${TEST_EXE8} "--test-suite=\"algorithms\"")