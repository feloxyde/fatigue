# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

ADD_TEST_SUITE(ts_fatigue_tutorial)

set (comp_py ${CMAKE_CURRENT_LIST_DIR}/../utils/compare.py)
set (test_tutorial_dir ${CMAKE_CURRENT_LIST_DIR})

# QUICK EXAMPLE

add_executable(test_tutorial_avg quick_example/main_avg.cpp)
target_link_libraries(test_tutorial_avg fatigue)

ADD_COVERED_TEST(test_avg_invokation
    CMD ${comp_py} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tutorial_avg ${test_tutorial_dir}/test_tutorial_avg.expected
    SUITE ts_fatigue_tutorial)


add_executable(test_tutorial_avg_chaining quick_example/main_avg_chaining.cpp)
target_link_libraries(test_tutorial_avg_chaining fatigue)
    
ADD_COVERED_TEST(test_avg_chaining_invokation
    CMD ${comp_py} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tutorial_avg_chaining ${test_tutorial_dir}/test_tutorial_avg.expected
    SUITE ts_fatigue_tutorial)

# SPECIFICATIONS

add_executable(test_tutorial_specs specifications/main_specs.cpp)
target_link_libraries(test_tutorial_specs fatigue)

ADD_COVERED_TEST(test_specs
    CMD ${comp_py} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tutorial_specs ${test_tutorial_dir}/test_tutorial_specs.expected
    SUITE ts_fatigue_tutorial)

# FIXTURES

add_executable(test_tutorial_fixtures fixtures/main_fixtures.cpp)
target_link_libraries(test_tutorial_fixtures fatigue)

ADD_COVERED_TEST(test_fixtures
    CMD ${comp_py} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tutorial_fixtures ${test_tutorial_dir}/test_tutorial_fixtures.expected
    SUITE ts_fatigue_tutorial)

# EXTENDING

add_executable(test_tutorial_ex extending/main_ex.cpp)
target_link_libraries(test_tutorial_ex fatigue)

ADD_COVERED_TEST(test_ex
    CMD ${comp_py} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tutorial_ex ${test_tutorial_dir}/test_tutorial_ex.expected
    SUITE ts_fatigue_tutorial)

ADD_COVERED_TEST(test_ex_fast
    CMD ${comp_py} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tutorial_ex -f" ${test_tutorial_dir}/test_tutorial_ex_fast.expected
    SUITE ts_fatigue_tutorial)


ADD_COVERED_TEST(test_ex_werror_default
    CMD ${comp_py} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tutorial_ex -w" ${test_tutorial_dir}/test_tutorial_ex_werror_default.expected
    SUITE ts_fatigue_tutorial)

ADD_COVERED_TEST(test_ex_werror_fast
    CMD ${comp_py} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tutorial_ex -f -w" ${test_tutorial_dir}/test_tutorial_ex_werror_fast.expected
    SUITE ts_fatigue_tutorial)
