# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

ADD_TEST_SUITE(ts_fatigue_utils_sanity)

set(utils_sanity_tests
    test_compareStrings.cpp
)

BUILD_TESTS_TO_SUITE(ts_fatigue_utils_sanity
TESTS ${utils_sanity_tests}
SOURCES ""
LIBS ""
)



set(comp_py ${CMAKE_CURRENT_LIST_DIR}/compare.py)

add_executable(test_compare_py test_compare_py.cpp)

ADD_COVERED_TEST(test_compare_py
    CMD ${comp_py} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_compare_py ${CMAKE_CURRENT_LIST_DIR}/test_compare_py.expected
    SUITE ts_fatigue_utils_sanity)

ADD_COVERED_TEST(test_compare_py_fail
    CMD ${comp_py} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_compare_py ${CMAKE_CURRENT_LIST_DIR}/test_compare_py_fail.expected
    SUITE ts_fatigue_utils_sanity)

set_property(TEST ts_fatigue_utils_sanity_test_compare_py_fail PROPERTY WILL_FAIL TRUE)

add_executable(test_assert test_assert.cpp)
ADD_COVERED_TEST(test_assert
    CMD ${comp_py} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_assert
    SUITE ts_fatigue_utils_sanity)


set_property(TEST ts_fatigue_utils_sanity_test_assert PROPERTY WILL_FAIL TRUE)