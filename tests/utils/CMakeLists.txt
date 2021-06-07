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
