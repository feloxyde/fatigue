# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

ADD_TEST_SUITE(ts_fatigue_unit_utils)

set(utils_tests
    demangle_nongnu.cpp
    demangle_gnu_fails.cpp
)


BUILD_TESTS_TO_SUITE(ts_fatigue_unit_utils
TESTS ${utils_tests}
SOURCES ""
LIBS fatigue
)
