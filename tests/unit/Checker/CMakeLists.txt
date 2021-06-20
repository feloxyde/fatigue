# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

ADD_TEST_SUITE(ts_fatigue_unit_Checker)

set(Checker_tests
    Check_move_construction.cpp
)

set_source_files_properties(Check_move_construction.cpp PROPERTIES COMPILE_FLAGS -Wno-pessimizing-move)

BUILD_TESTS_TO_SUITE(ts_fatigue_unit_Checker
TESTS ${Checker_tests}
SOURCES ""
LIBS fatigue
)
