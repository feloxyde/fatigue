option(ENABLE_LLVM_COVERAGE "Enable llvm coverage. Needs clang++ and/or clang to be selected as compiler, and llvm-profdata, llvm-cov and llvm-cxxflit to be installed and invokable as-is.")
option(ENABLE_LLVM_COVERAGE_INSTANTIATION "Enable reporting llvm coverage with instantiation details, needs ENABLE_LLVM_COVERAGE to be set to ON and proper configuration for it.")



function(ADD_COVERED_TEST test_name)
    set(options "")
    set(oneValueArgs CMD)
    set(multiValuesArgs "")
    set(ARGP ${test_name}_ARGS)
    cmake_parse_arguments(${ARGP} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN}) 
    add_test(NAME ${test_name} COMMAND env LLVM_PROFILE_FILE=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${full_tname}.profraw  ${${ARGP}_CMD})
    message("coverage output file")
    message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${full_tname}.profraw)
endfunction()


#this adds a test_suite from a directory
#params are :  
#TESTS [list] : a list of all test files that are in the suite
#SOURCES [list] : a list of all sources that are needed to link with test
#LIBS [list] a list of all libs that has to be linked to tests
#COVERING [list] : a list of all sources, hpp included, to add to the suite for coverage
function(ADD_TEST_SUITE suite_name)
#parsing arguments
    set(options "")
    set(oneValueArgs "")
    set(multiValuesArgs TESTS SOURCES LIBS)
    set(ARGP ${suite_name}_ARGS)
    cmake_parse_arguments(${ARGP} "${options}" "${oneValueArgs}" "${multiValuesArgs}" ${ARGN})

    add_custom_target(${suite_name}_testr COMMAND ctest -R "${suite_name}_*")   
    add_custom_target(${suite_name}_testo COMMAND ctest --output-on-failure -R "${suite_name}_*")   
    #PARSE ALL TEST FILES
    foreach(testf ${${ARGP}_TESTS})
        #creating test executable and linking
        get_filename_component(tname ${testf} NAME_WE)
        set(full_tname "${suite_name}_${tname}")
        #converting sources into correctly presented sources
        foreach(source ${${ARGP}_SOURCES})
            list(APPEND local_source "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
        endforeach()
        add_executable(${full_tname} ${CMAKE_CURRENT_SOURCE_DIR}/${testf})
        target_link_libraries(${full_tname} ${${ARGP}_LIBS})
        ADD_COVERED_TEST(${full_tname} CMD ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${full_tname})
        add_dependencies(${suite_name}_testr ${full_tname})
        add_dependencies(${suite_name}_testo ${full_tname})

    endforeach()
    add_custom_target(${suite_name}_coverage )

endfunction(ADD_TEST_SUITE)

function(ADD_COVERAGE_REPORT suite_name)
#parsing arguments
    set(options "")
    set(oneValueArgs CMD BIN)
    set(multiValuesArgs COVERING TESTS)
    set(ARGP ${suite_name}_ARGS)
    cmake_parse_arguments(${ARGP} "${options}" "${oneValueArgs}" "${multiValuesArgs}" ${ARGN})
    
    #creating a target to run all tests from the suite
    
    #building up a list of sources 
    foreach(cvrd ${${ARGP}_BIN})
        set(${suite_name}_covered ${${suite_name}_covered} ${CMAKE_CURRENT_SOURCE_DIR}/${cvrd})
    endforeach()

    foreach(tst ${${ARGP}_TESTS})
        set(${suite_name}_covering_tests ${${suite_name}_covering_tests} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${tst}.profraw)
    endforeach()

    add_custom_target(${suite_name}_profdata COMMAND llvm-profdata merge ${${suite_name}_covering_tests} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${suite_name}.profdata)
    add_custom_target(${suite_name}_txt COMMAND llvm-cov report --instr-profile=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${suite_name}.profdata  $<TARGET_FILE:${${ARGP}_BIN}> ${${suite_name}_covered} > ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${suite_name}.txt DEPENDS ${suite_name}_profdata)
    if(ENABLE_LLVM_COVERAGE_INSTANTIATION)
        set(llvmcovshowflags --show-line-counts-or-regions --format html --use-color -Xdemangler llvm-cxxfilt --show-instantiations)
    else()
        set(llvmcovshowflags --show-line-counts-or-regions --format html --use-color -Xdemangler llvm-cxxfilt --show-instantiations=false)
    endif()
    add_custom_target(${suite_name}_html COMMAND llvm-cov show --instr-profile=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${suite_name}.profdata  $<TARGET_FILE:${${ARGP}_BIN}> ${${suite_name}_covered} ${llvmcovshowflags} > ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${suite_name}.html DEPENDS ${suite_name}_profdata)
endfunction()
