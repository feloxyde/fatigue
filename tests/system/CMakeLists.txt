ADD_TEST_SUITE(ts_fatigue_system)

add_executable(test_fatigue test_fatigue.cpp)
target_link_libraries(test_fatigue fatigue)

set (comp_py ${CMAKE_CURRENT_LIST_DIR}/compare.py)
set (test_sys_dir ${CMAKE_CURRENT_LIST_DIR})


ADD_COVERED_TEST(test_fatigue_cmdline 
    CMD ${comp_py} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue ${test_sys_dir}/test_fatigue_cmdline.expected
    SUITE ts_fatigue_system)
    
ADD_COVERED_TEST(test_exclude_ostream 
    CMD ${comp_py} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue --exclude 'suite3//.*|suite2.*//.*:0'" ${test_sys_dir}/test_exclude_ostream.expected
    SUITE ts_fatigue_system)

ADD_COVERED_TEST(test_select_ostream 
    CMD ${comp_py} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue --select 'suite3//.*|suite2.*//.*:0'" ${test_sys_dir}/test_select_ostream.expected
    SUITE ts_fatigue_system)
    
ADD_COVERED_TEST(test_select_exclude_ostream 
    CMD ${comp_py} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue --select 'suite3//.*|suite2.*//.*:0' --exclude '.*3:0.*'" ${test_sys_dir}/test_select_exclude_ostream.expected
    SUITE ts_fatigue_system)
