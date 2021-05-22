# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

ADD_TEST_SUITE(ts_fatigue_unit_Config)

set(unit_tests
    Filter.cpp
)


BUILD_TESTS_TO_SUITE(ts_fatigue_unit_Config
TESTS ${unit_tests}
SOURCES ""
LIBS fatigue
)
