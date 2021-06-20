# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20.0)

#project declaration
project(fatigue_toplevel)


include(ProcessorCount)
ProcessorCount(N_PROC)
if(N_PROC EQUAL 0)
     set(N_PROC 1)
endif()

#testing enabled, comment to disable
enable_testing()
add_custom_target(testo COMMAND ctest --output-on-failure)  

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Setting compiler flags and build types
SET(CMAKE_CXX_FLAGS "-Wall -Wextra")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g -Werror")

include(${CMAKE_CURRENT_LIST_DIR}/cmake_llvm_coverage/LLVMCoverage.cmake)
#include(CMakeUtils.cmake)

set(CMAKE_CXX_STANDARD 20)

#to generate with proper flags, please use 
#cmake .. -DENABLE_LLVM_COVERAGE=ON -DENABLE_GDB=ON -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

if(ENABLE_LLVM_COVERAGE)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping -ftest-coverage")
endif()

#setting the root directory for includes as the root of the project, 
#so files can be included as <skuldenka/...>
include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR})


file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/fatigue/external)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/cxxopts/include/cxxopts.hpp DESTINATION ${CMAKE_CURRENT_LIST_DIR}/fatigue/external)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/fatigue)

#installing headers
#set_target_properties(fatigue PROPERTIES PUBLIC_HEADER "${headers}")
install(DIRECTORY fatigue/ DESTINATION ${CMAKE_INSTALL_PREFIX}/fatigue/fatigue)
install(FILES fatigue-config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/fatigue)
#installing lib

########################################################### 
########## TESTING ########################################
###########################################################


add_subdirectory(tests)

get_property(unit_binaries GLOBAL PROPERTY ts_fatigue_unit_BINARIES_LIST)

file(GLOB_RECURSE sources LIST_DIRECTORIES false RELATIVE ${CMAKE_SOURCE_DIR} fatigue/*.[ch]pp )
list(FILTER sources EXCLUDE REGEX fatigue/external/*)


ADD_COVERAGE_REPORT(coverage_all COVERING ${sources} TESTS * BINARIES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue ${unit_binaries}) 

########################################################### 
########## QUALITY ########################################
###########################################################

set(QUALITY_DIR ${CMAKE_BINARY_DIR}/quality)
set(QUALITY_REPORT_DIR ${QUALITY_DIR}/reports)
set(COMPILE_COMMANDS ${CMAKE_BINARY_DIR}/compile_commands.json)

file(MAKE_DIRECTORY ${QUALITY_REPORT_DIR})


set(QUALITY_CODECHECKER_DIR ${QUALITY_DIR}/codechecker)
set(QUALITY_CODECHECKER_SKIPFILE ${CMAKE_SOURCE_DIR}/quality/codechecker_skipfile.txt)
file(MAKE_DIRECTORY ${QUALITY_CODECHECKER_DIR})

add_custom_target(quality_all)

add_custom_target(quality_codechecker_sensitive 
                COMMAND codechecker analyze ${COMPILE_COMMANDS} --enable sensitive --disable google-runtime-references --ignore ${QUALITY_CODECHECKER_SKIPFILE} --output ${QUALITY_CODECHECKER_DIR}/sentitive || true
                COMMAND codechecker parse --export html --output ${QUALITY_REPORT_DIR}/codechecker_sensitive ${QUALITY_CODECHECKER_DIR}/codechecker_sentitive)

add_custom_target(quality_codechecker_sensitive_ctu
                COMMAND codechecker analyze ${COMPILE_COMMANDS} --enable sensitive --ctu --disable google-runtime-references --ignore ${QUALITY_CODECHECKER_SKIPFILE} --output ${QUALITY_CODECHECKER_DIR}/sentitive_ctu || true
                COMMAND codechecker parse --export html --output ${QUALITY_REPORT_DIR}/codechecker_sensitive_ctu ${QUALITY_CODECHECKER_DIR}/codechecker_sentitive_ctu)

add_custom_target(quality_codechecker_extreme
                COMMAND codechecker analyze ${COMPILE_COMMANDS} --enable extreme --disable google-runtime-references --ignore ${QUALITY_CODECHECKER_SKIPFILE} --output ${QUALITY_CODECHECKER_DIR}/extreme || true
                COMMAND codechecker parse --export html --output ${QUALITY_REPORT_DIR}/codechecker_extreme ${QUALITY_CODECHECKER_DIR}/codechecker_extreme)

add_custom_target(quality_codechecker_extreme_ctu
                COMMAND codechecker analyze ${COMPILE_COMMANDS} --enable extreme --ctu --disable google-runtime-references --ignore ${QUALITY_CODECHECKER_SKIPFILE} --output ${QUALITY_CODECHECKER_DIR}/extreme_ctu || true
                COMMAND codechecker parse --export html --output ${QUALITY_REPORT_DIR}/codechecker_extreme_ctu ${QUALITY_CODECHECKER_DIR}/codechecker_extreme_ctu)

set(QUALITY_CPPCHECK_DIR ${QUALITY_DIR}/cppcheck)
file(MAKE_DIRECTORY ${QUALITY_CPPCHECK_DIR})

add_custom_target(quality_cppcheck_default
                COMMAND cppcheck --project=${COMPILE_COMMANDS} --xml --output-file=${QUALITY_CPPCHECK_DIR}/default.xml
                COMMAND cppcheck-htmlreport --title=DefaultReport --file=${QUALITY_CPPCHECK_DIR}/default.xml --report-dir=${QUALITY_REPORT_DIR}/cppcheck_default)

add_custom_target(quality_cppcheck_all
                COMMAND cppcheck --project=${COMPILE_COMMANDS} --enable=all --xml --output-file=${QUALITY_CPPCHECK_DIR}/all.xml
                COMMAND cppcheck-htmlreport --title=AllEnabledReport --file=${QUALITY_CPPCHECK_DIR}/all.xml --report-dir=${QUALITY_REPORT_DIR}/cppcheck_all)

add_custom_target(quality_cppcheck_bug_hunting
                COMMAND cppcheck --project=${COMPILE_COMMANDS} --bug-hunting --enable=all --inconclusive --xml --output-file=${QUALITY_CPPCHECK_DIR}/bug_hunting.xml
                COMMAND cppcheck-htmlreport --title=AllEnabledReport --file=${QUALITY_CPPCHECK_DIR}/bug_hunting.xml --report-dir=${QUALITY_REPORT_DIR}/cppcheck_bug_hunting)

add_dependencies(quality_all
                quality_codechecker_sensitive
                quality_codechecker_sensitive_ctu
                quality_codechecker_extreme
                quality_codechecker_extreme_ctu
                quality_cppcheck_default
                quality_cppcheck_all
                quality_cppcheck_bug_hunting 
)