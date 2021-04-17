#THIS FILE checks for installation of lib fatigue by rebuilding all tests without building fatigue.

cmake_minimum_required(VERSION 3.1.0)

#project declaration (noodle engine)
project(fatigue_install_tests)


enable_testing()
add_custom_target(testo COMMAND ctest --output-on-failure)  

set(CMAKE_CXX_STANDARD 20)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include(${CMAKE_CURRENT_LIST_DIR}/../../cmake_llvm_coverage/LLVMCoverage.cmake)


set(INSTALL_CHECK true) 

add_subdirectory(.. tests/)

