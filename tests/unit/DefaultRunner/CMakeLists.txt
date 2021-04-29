
ADD_TEST_SUITE(ts_fatigue_unit_DefaultRunner)

set(DefaultRunner_tests
    DefaultLogger_noparam.cpp 
    noparam.cpp
    exception.cpp
    fatal.cpp
    load_failure.cpp
    passing.cpp
)


BUILD_TESTS_TO_SUITE(ts_fatigue_unit_DefaultRunner
TESTS ${DefaultRunner_tests}
SOURCES ""
LIBS fatigue
)
