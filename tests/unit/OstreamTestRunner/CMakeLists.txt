
ADD_TEST_SUITE(ts_fatigue_unit_OstreamTestRunner)

set(OstreamTestRunner_tests
    OstreamTestLogger_noparam.cpp 
    noparam.cpp
    exception.cpp
    fatal.cpp
    load_failure.cpp
    passing.cpp
)


BUILD_TESTS_TO_SUITE(ts_fatigue_unit_OstreamTestRunner
TESTS ${OstreamTestRunner_tests}
SOURCES ""
LIBS fatigue
)
