
ADD_TEST_SUITE(ts_fatigue_unit_DefaultRunner)

set(DefaultRunner_tests
    DefaultLogger_noparam.cpp 
    endRunOnFailure.cpp
    endRunOnSuccess.cpp
    exception.cpp
    load_failure.cpp
    noparam.cpp
    passing.cpp
)


BUILD_TESTS_TO_SUITE(ts_fatigue_unit_DefaultRunner
TESTS ${DefaultRunner_tests}
SOURCES ""
LIBS fatigue
)
