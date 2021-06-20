# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20.0)

#project declaration
project(fatigue)


include(ProcessorCount)
ProcessorCount(N_PROC)
if(N_PROC EQUAL 0)
     set(N_PROC 1)
endif()

set(CMAKE_CXX_STANDARD 20)

set(headers 
    Checker.hpp
    Config.hpp
    fatigue.hpp
    Runner.hpp
    runners/DefaultRunner.hpp
    Suite.hpp
    Test.hpp
    utils.hpp
)

set(headers_external
    external/cxxopts.hpp
)

set(sources
    Checker.cpp
    Config.cpp
    fatigue.cpp
    Runner.cpp
    runners/DefaultRunner.cpp
    Suite.cpp
)

add_library(fatigue ${sources} ${headers})