

set(unit_tests
unit/test_Config.cpp
unit/test_Filter.cpp
unit/test_OstreamTestLogger_noparam.cpp 
unit/test_OstreamTestRunner_noparam.cpp
unit/test_OstreamTestRunner_exception.cpp
unit/test_OstreamTestRunner_fatal.cpp
unit/test_OstreamTestRunner_load_failure.cpp
unit/test_OstreamTestRunner_passing.cpp
unit/test_Test_automsg_fail_showtypes.cpp
unit/test_Test_automsg_fail_showtypes_shownames.cpp
unit/test_Test_automsg_fail.cpp
unit/test_Test_automsg_pass.cpp
#test_TestDriver.cpp
)

ADD_TEST_SUITE(ts_fatigue)

BUILD_TESTS_TO_SUITE(ts_fatigue
TESTS ${unit_tests}
SOURCES ""
LIBS fatigue
)

add_executable(test_fatigue system/test_fatigue.cpp)
target_link_libraries(test_fatigue fatigue)

set (comp_py ${CMAKE_CURRENT_LIST_DIR}/system/compare.py)
set (test_sys_dir ${CMAKE_CURRENT_LIST_DIR}/system)

ADD_COVERED_TEST(test_fatigue_cmdline CMD ${comp_py} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue ${test_sys_dir}/test_fatigue_cmdline.expected)
ADD_COVERED_TEST(test_exclude_ostream CMD ${comp_py} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue --exclude 'suite3//.*|suite2.*//.*:0'" ${test_sys_dir}/test_exclude_ostream.expected)
ADD_COVERED_TEST(test_select_ostream CMD ${comp_py} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue --select 'suite3//.*|suite2.*//.*:0'" ${test_sys_dir}/test_select_ostream.expected)
ADD_COVERED_TEST(test_select_exclude_ostream CMD ${comp_py} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue --select 'suite3//.*|suite2.*//.*:0' --exclude '.*2:0.*'" ${test_sys_dir}/test_select_exclude_ostream.expected)


