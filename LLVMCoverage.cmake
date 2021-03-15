option(ENABLE_LLVM_COVERAGE "Enable llvm coverage. Needs clang++ and/or clang to be selected as compiler, and llvm-profdata, llvm-cov and llvm-cxxflit to be installed and invokable as-is.")
option(ENABLE_LLVM_COVERAGE_INSTANTIATION "Enable reporting llvm coverage with instantiation details, needs ENABLE_LLVM_COVERAGE to be set to ON and proper configuration for it.")



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
    set(multiValuesArgs TESTS SOURCES LIBS COVERING)
    set(ARGP ${suite_name}_ARGS)
    cmake_parse_arguments(${ARGP} "${options}" "${oneValueArgs}" "${multiValuesArgs}" ${ARGN})
    
    
    add_custom_target(${suite_name}_coverage_profraws)

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
        add_test(NAME ${full_tname} COMMAND ${full_tname})
        add_dependencies(${suite_name}_testr ${full_tname})
        add_dependencies(${suite_name}_testo ${full_tname})
        if(ENABLE_LLVM_COVERAGE)
            #targets to build profiles data
            add_custom_target(${full_tname}_profraw COMMAND LLVM_PROFILE_FILE=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${full_tname}.profraw ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${full_tname} || true DEPENDS ${full_tname})
            add_dependencies(${suite_name}_coverage_profraws ${full_tname}_profraw)
            #listing profiles for coverage report
            set(${suite_name}_profraws ${${suite_name}_profraws} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${full_tname}.profraw)

            #registering to list of tests execs for llvm report and show commands
            if(NOT ${suite_name}_TEST_FIRST)
                set(${suite_name}_TEST_FIRST 1)
                set(${suite_name}_tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${full_tname})
            else ()
            set(${suite_name}_tests ${${suite_name}_tests} -object ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${full_tname})
            endif()
        endif()
    endforeach()


    #creating a target to run all tests from the suite
    
    if(ENABLE_LLVM_COVERAGE)
        #gathering profraws and exporting report

        #building up a list of sources 
        foreach(cvrd ${${ARGP}_COVERING})
            set(${suite_name}_covered ${${suite_name}_covered} ${CMAKE_CURRENT_SOURCE_DIR}/${cvrd})
        endforeach()

        add_custom_target(${suite_name}_coverage_profdata COMMAND llvm-profdata merge ${${suite_name}_profraws} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${suite_name}.profdata DEPENDS ${suite_name}_coverage_profraws)
        add_custom_target(${suite_name}_coverage_report COMMAND llvm-cov report --instr-profile=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${suite_name}.profdata  ${${suite_name}_tests} ${${suite_name}_covered} > ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${suite_name}_coverage.txt DEPENDS ${suite_name}_coverage_profdata)
        if(ENABLE_LLVM_COVERAGE_INSTANTIATION)
            set(llvmcovshowflags --show-line-counts-or-regions --format html --use-color -Xdemangler llvm-cxxfilt --show-instantiations)
        else()
            set(llvmcovshowflags --show-line-counts-or-regions --format html --use-color -Xdemangler llvm-cxxfilt --show-instantiations=false)
        endif()
        add_custom_target(${suite_name}_coverage_html COMMAND llvm-cov show --instr-profile=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${suite_name}.profdata  ${${suite_name}_tests} ${${suite_name}_covered} ${llvmcovshowflags} > ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${suite_name}_coverage.html DEPENDS ${suite_name}_coverage_profdata)

        ##REGISTER SOURCES AND PROFILES INTO A GLOBAL VAR FOR GENERAL COVERAGE !
    endif()

    add_custom_target(${suite_name}_coverage )


endfunction(ADD_TEST_SUITE)