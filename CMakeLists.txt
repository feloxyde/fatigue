#[[
Copyright or © or Copr. Félix Bertoni (2020)

felix.bertoni987@gmail.com

This software is a computer program whose purpose is to provide an easy way 
to create softwares with a modulare design.

This software is governed by the CeCILL-C license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-C
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-C license and that you accept its terms.
]]




cmake_minimum_required(VERSION 3.1.0)

#project declaration (noodle engine)
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

set(headers 
    fatigue/Checker.hpp
    fatigue/Config.hpp
    fatigue/fatigue.hpp
    fatigue/runners/DefaultRunner.hpp
    fatigue/Suite.hpp
    fatigue/Test.hpp
    fatigue/Runner.hpp
    fatigue/utils.hpp
)

set(sources
    fatigue/Checker.cpp
    fatigue/Config.cpp
    fatigue/fatigue.cpp
    fatigue/runners/DefaultRunner.cpp
    fatigue/Suite.cpp
)

add_library(fatigue ${sources})



set_target_properties(fatigue PROPERTIES PUBLIC_HEADER "${headers}")

install(TARGETS fatigue PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fatigue)

########################################################### 
########## TESTING ########################################
###########################################################


add_subdirectory(tests)

get_property(unit_binaries GLOBAL PROPERTY ts_fatigue_unit_BINARIES_LIST)

ADD_COVERAGE_REPORT(coverage_all COVERING ${headers} ${sources} TESTS * BINARIES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fatigue ${unit_binaries} COVERING ${headers} ${sources})