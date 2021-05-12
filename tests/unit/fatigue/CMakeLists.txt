
ADD_TEST_SUITE(ts_fatigue_unit_fatigue)

set(fatigue_tests
    unsupported_runner_option.cpp
)


BUILD_TESTS_TO_SUITE(ts_fatigue_unit_fatigue
TESTS ${fatigue_tests}
SOURCES ""
LIBS fatigue
)
