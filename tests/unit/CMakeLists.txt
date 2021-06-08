# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

ADD_TEST_SUITE(ts_fatigue_unit)

add_subdirectory(Checker)
add_subdirectory(Config)
add_subdirectory(DefaultRunner)
add_subdirectory(Test)
add_subdirectory(fatigue)
add_subdirectory(utils)

TEST_SUITE_IMPORT_BINARIES(ts_fatigue_unit ts_fatigue_unit_Config)
TEST_SUITE_IMPORT_BINARIES(ts_fatigue_unit ts_fatigue_unit_DefaultRunner)
TEST_SUITE_IMPORT_BINARIES(ts_fatigue_unit ts_fatigue_unit_Test)
TEST_SUITE_IMPORT_BINARIES(ts_fatigue_unit ts_fatigue_unit_fatigue)