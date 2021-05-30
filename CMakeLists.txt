# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.18.0)

#project declaration
project(fatigue)


include(ProcessorCount)
ProcessorCount(N_PROC)
if(N_PROC EQUAL 0)
     set(N_PROC 1)
endif()


include(GNUInstallDirs)

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
include_directories(${CMAKE_CURRENT_LIST_DIR})


file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/fatigue/external)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/cxxopts/include/cxxopts.hpp DESTINATION ${CMAKE_CURRENT_LIST_DIR}/fatigue/external)

set(headers 
    fatigue/Checker.hpp
    fatigue/Config.hpp
    fatigue/fatigue.hpp
    fatigue/Runner.hpp
    fatigue/runners/DefaultRunner.hpp
    fatigue/Suite.hpp
    fatigue/Test.hpp
    fatigue/utils.hpp
)

set(headers_external
    fatigue/external/cxxopts.hpp
)

set(sources
    fatigue/Checker.cpp
    fatigue/Config.cpp
    fatigue/fatigue.cpp
    fatigue/Runner.cpp
    fatigue/runners/DefaultRunner.cpp
    fatigue/Suite.cpp
)

add_library(fatigue ${sources})


#installing headers
#set_target_properties(fatigue PROPERTIES PUBLIC_HEADER "${headers}")
install(DIRECTORY fatigue/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fatigue
        FILES_MATCHING PATTERN "*.hpp")
#installing lib
install(TARGETS fatigue)

########################################################### 
########## TESTING ########################################
###########################################################


add_subdirectory(tests)

get_property(unit_binaries GLOBAL PROPERTY ts_fatigue_unit_BINARIES_LIST)

ADD_COVERAGE_REPORT(coverage_all COVERING ${headers} ${sources} TESTS * BINARIES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue ${unit_binaries} COVERING ${headers} ${sources}) 



########################################################### 
########## QUALITY ########################################
###########################################################

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/quality/codechecker)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/quality/reports)


add_custom_target(codechecker_sensitive 
                COMMAND codechecker analyze ${CMAKE_BINARY_DIR}/compile_commands.json --enable sensitive --disable google-runtime-references --ignore ${CMAKE_SOURCE_DIR}/quality/codechecker_skipfile.txt --output ${CMAKE_BINARY_DIR}/quality/codechecker/sentitive || true
                COMMAND codechecker parse --export html --output ${CMAKE_BINARY_DIR}/quality/reports/codechecker_sensitive ${CMAKE_BINARY_DIR}/quality/codechecker/sentitive)

add_custom_target(codechecker_extreme
                COMMAND codechecker analyze ${CMAKE_BINARY_DIR}/compile_commands.json --enable extreme --disable google-runtime-references --ignore ${CMAKE_SOURCE_DIR}/quality/codechecker_skipfile.txt --output ${CMAKE_BINARY_DIR}/quality/codechecker/extreme || true
                COMMAND codechecker parse --export html --output ${CMAKE_BINARY_DIR}/quality/reports/codechecker_extreme ${CMAKE_BINARY_DIR}/quality/codechecker/extreme)

