# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

ADD_TEST_SUITE(ts_fatigue_unit_fatigue)

set(fatigue_tests
    unsupported_runner_option.cpp
    runner_not_found.cpp
)


BUILD_TESTS_TO_SUITE(ts_fatigue_unit_fatigue
TESTS ${fatigue_tests}
SOURCES ""
LIBS fatigue
)
