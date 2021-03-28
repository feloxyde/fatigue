option(ENABLE_LLVM_COVERAGE "Enable llvm coverage. Needs clang++ and/or clang to be selected as compiler, and llvm-profdata, llvm-cov and llvm-cxxflit to be installed and invokable as-is.")
option(ENABLE_LLVM_COVERAGE_INSTANTIATION "Enable reporting llvm coverage with instantiation details, needs ENABLE_LLVM_COVERAGE to be set to ON and proper configuration for it.")


set(LLVM_COVERAGE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/llvmcoverage)

function(ADD_COVERED_TEST test_name)
    set(options "")
    set(oneValueArgs CMD SUITE)
    set(multiValuesArgs "")
    set(ARGP ${test_name}_ARGS)
    cmake_parse_arguments(${ARGP} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN}) 
    if(NOT DEFINED ${ARGP}_SUITE)
        set(full_tname "${test_name}")
    else()
        set(full_tname "${${ARGP}_SUITE}_${test_name}")
    endif()
    add_test(NAME ${full_tname} COMMAND env LLVM_PROFILE_FILE=${LLVM_COVERAGE_OUTPUT_DIR}/${full_tname}.profraw  ${${ARGP}_CMD})
endfunction()
ù
function(ADD_TEST_SUITE test_suite)
    add_custom_target(${suite_name}_testr COMMAND ctest -R "${suite_name}_*")   
    add_custom_target(${suite_name}_testo COMMAND ctest --output-on-failure -R "${suite_name}_*")   
endfunction()

#this allow to create several tests from C++ sources at once and add them to suite
#params are :  
#TESTS [list] : a list of all test files to add
#SOURCES [list] : a list of all sources that are needed to be used to build with tests
#LIBS [list] a list of all libs that has to be linked to tests
function(BUILD_TESTS_TO_SUITE suite_name)
#parsing arguments
    set(options "")
    set(oneValueArgs "")
    set(multiValuesArgs TESTS SOURCES LIBS)
    set(ARGP ${suite_name}_ARGS)
    cmake_parse_arguments(${ARGP} "${options}" "${oneValueArgs}" "${multiValuesArgs}" ${ARGN})
    #PARSE ALL TEST FILES
    foreach(testf ${${ARGP}_TESTS})
        #creating test executable and linking
        get_filename_component(tname ${testf} NAME_WE)
        set(full_tname "${suite_name}_${tname}")
        #converting sources into correctly presented sources
        foreach(source ${${ARGP}_SOURCES})
            list(APPEND local_source "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
        endforeach()
        add_executable(${full_tname} ${CMAKE_CURRENT_SOURCE_DIR}/${testf} ${local_source})
        target_link_libraries(${full_tname} ${${ARGP}_LIBS})
        ADD_COVERED_TEST(${full_tname} CMD ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${full_tname} SUITE ${suite_name})
    endforeach()
endfunction(ADD_TEST_SUITE)


function(ADD_COVERAGE_REPORT report_name)
#parsing arguments
    set(options "")
    set(oneValueArgs CMD BIN)
    set(multiValuesArgs COVERING TESTS)
    set(ARGP ${report_name}_ARGS)
    cmake_parse_arguments(${ARGP} "${options}" "${oneValueArgs}" "${multiValuesArgs}" ${ARGN})
    
    #creating a target to run all tests from the suite
    
    #building up a list of sources 
    foreach(cvrd ${${ARGP}_BIN})
        set(${report_name}_covered ${${report_name}_covered} ${CMAKE_CURRENT_SOURCE_DIR}/${cvrd})
    endforeach()

    foreach(tst ${${ARGP}_TESTS})
        set(${report_name}_covering_tests ${${report_name}_covering_tests} ${LLVM_COVERAGE_OUTPUT_DIR}/${tst}.profraw)
    endforeach()

    add_custom_target(${report_name}_profdata COMMAND llvm-profdata merge ${${report_name}_covering_tests} -o ${LLVM_COVERAGE_OUTPUT_DIR}/${suite_name}.profdata)
    add_custom_target(${report_name}_txt COMMAND llvm-cov report --instr-profile=${LLVM_COVERAGE_OUTPUT_DIR}/${report_name}.profdata  $<TARGET_FILE:${${ARGP}_BIN}> ${${suite_name}_covered} > ${LLVM_COVERAGE_OUTPUT_DIR}/${suite_name}.txt DEPENDS ${suite_name}_profdata)
    if(ENABLE_LLVM_COVERAGE_INSTANTIATION)
        set(llvmcovshowflags --show-line-counts-or-regions --format html --use-color -Xdemangler llvm-cxxfilt --show-instantiations)
    else()
        set(llvmcovshowflags --show-line-counts-or-regions --format html --use-color -Xdemangler llvm-cxxfilt --show-instantiations=false)
    endif()
    add_custom_target(${report_name}_html COMMAND llvm-cov show --instr-profile=${LLVM_COVERAGE_OUTPUT_DIR}/${report_name}.profdata  $<TARGET_FILE:${${ARGP}_BIN}> ${${suite_name}_covered} ${llvmcovshowflags} > ${LLVM_COVERAGE_OUTPUT_DIR}/${suite_name}.html DEPENDS ${suite_name}_profdata)
endfunction()
