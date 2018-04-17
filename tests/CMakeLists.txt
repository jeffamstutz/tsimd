## ========================================================================== ##
## The MIT License (MIT)                                                      ##
##                                                                            ##
## Copyright (c) 2017 Intel Corporation                                       ##
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

# precompile CATCH2 main() function
add_library(tsimd_catch_main STATIC catch/catch_main.cpp)

# this macro encapsulates the common code for defining a unique test
macro(tsimd_add_pack_test TEST_NAME TEST_WIDTH TEST_DOUBLE)
  add_executable(test_pack${TEST_NAME} test_pack.cpp)

  target_compile_definitions(test_pack${TEST_NAME} PRIVATE
                             -DTEST_WIDTH=${TEST_WIDTH})
  target_compile_definitions(test_pack${TEST_NAME} PRIVATE
                             -DTEST_DOUBLE_PRECISION=${TEST_DOUBLE})
  target_link_libraries(test_pack${TEST_NAME} tsimd_catch_main)

  set(TEST_EXE ${EXECUTABLE_OUTPUT_PATH}/test_pack${TEST_NAME})

  add_test(bit_iterator${TEST_NAME}         ${TEST_EXE} "[bit_iterator]")
  add_test(member_functions${TEST_NAME}     ${TEST_EXE} "[member_functions]")
  add_test(arithmetic_operators${TEST_NAME} ${TEST_EXE} "[arithmetic_operators]")
  add_test(bitwise_operators${TEST_NAME}    ${TEST_EXE} "[bitwise_operators]")
  add_test(logic_operators${TEST_NAME}      ${TEST_EXE} "[logic_operators]")
  add_test(math_functions${TEST_NAME}       ${TEST_EXE} "[math_functions]")
  add_test(algorithms${TEST_NAME}           ${TEST_EXE} "[algorithms]")
  add_test(random${TEST_NAME}               ${TEST_EXE} "[random]")
  add_test(memory_operations${TEST_NAME}    ${TEST_EXE} "[memory_operations]")
endmacro()

# define the tests
foreach(test_width 1 4 8 16)
  foreach(test_double 0 1)
    math(EXPR bit_width "(${test_double}+1)*32")
    tsimd_add_pack_test(${bit_width}x${test_width} ${test_width} ${test_double})
  endforeach(test_double)
endforeach(test_width)
