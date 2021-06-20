# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

ADD_TEST_SUITE(ts_fatigue_unit_Test)

set(Test_tests
    automsg_fail.cpp
    automsg_pass.cpp
)


BUILD_TESTS_TO_SUITE(ts_fatigue_unit_Test
TESTS ${Test_tests}
SOURCES ""
LIBS fatigue
)
