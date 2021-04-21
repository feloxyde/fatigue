# Contributing to fatigue

Contributions are of course welcome, however, please carefully read this document before contributing, so you don't waste your time.

[[_TOC_]]

## Contribution process 

You are highly encouraged to inform me at  [felix.bertoni987@gmail.com](mailto:felix.bertoni987@gmail.com) if you fork or intend to contribute to the project, 
so we can discuss about it. 

Contributions are supposed to be forked from ```dev``` branch and merged back in it when done. All tests and full CI shall be passing to merge.

In case you are not sure about what you've done, or if you want to have feedback during your implementation, simply start a draft merge request.

## Where to contribute ?

There are several ways in which you can contribute to fatigue. 

- Communicate : talk about fatigue to your friends and colleagues.
- Feedbacks : comment on feedback-tagged issues to give your opinion about some points, or ideas. You can also simply star the project, which is motivating.
- Devops : help setting up the project by either providing tools or setting them up, especially for code quality.
- Testing : write more tests to enhance reliability of the project
- Development : add features, fix bugs or review code.
- Documenting : help others using this by documenting the project and reviewing current documentation.

## Code formatting and style

Any C/C++ code shall be formatted with the ```.clang-format``` at the root of the directory, which is LLVM style with few tweaks. 

All code has to be encased within ```ftg``` namespace. 

- ```UpperCamelCase``` is used for classes, structs and templates parameters.
- ```UPPER_UNDERSCORE_CASE``` is used for macros. But there should be no macro in code, unless they are absolutely needed for a relevant feature, as for example portability.
- ```lowerCamelCase``` is used for everything else. 

Be concise in your code and try to use convenient features as much as possible, as for example *concepts*.

## Testing and architecture

Sources files, both headers and definitions, shall go into ```fatigue/``` directory. Headers have to be listed in the CMake file as headers of ```fatigue``` library.

Decouple your code as much as possible to allow flexibility and refined unit or integration testing. 

As an example, instead of 

```cpp
void runTest(){
    std::cout << ...;
}
```
one shall write

```cpp
void runTest(std::ostream& out){
    out << ...;
}
```

Behavior not involving IO shall be inforced with unit and integration testing as much as possible.

Additionally, system level testing is welcome, and is mandatory for behavior involving IO.

Tests shall be registered using CMake utility ```LLVMCoverage.cmake``` defined in submodule ```cmake_llvm_coverage```. 
Additionally, tests shall refer to sources as global includes ```#include <...>``` to allow post-install testing.

## Quality

For now, code quality requirements are decided by human review. Automated tools will be added later. 
If you follow C++ Code Guidelines, it should be OK.

Additionally, when compiling with ```-Wall -Werror```, it shall not trigger any warning. Those flags are activated in all cmake build type.

# Development environment

In order to build the program for development, you will need : 

- **clang** or **clang++** as compiler to compile with clang coverage
- **llvm-profdata** to process raw coverage files
- **llvm-cov** to generate report
- **llvm-cxxflit** to demangle names, especially templates ones

## Building as debug and testing

To build in debug mode, configure CMake with 

```
cmake .. -DCMAKE_BUILD_TYPE
```

It will enable ```-g -O0 -Werror``` flags.

Target ```test``` runs all tests, while ```testo``` runs all tests with ```--output-on-failure``` enabled.

## Coverage

To build for coverage, configure CMake with 

```
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_LLVM_COVERAGE=ON -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang
```

Build all targets and run tests.

```
make
make test
```

It should create a ```llvmcoverage``` directory in the current build directory, with ```.profraw``` files in it corresponding to each test. 

Finally, there are two coverage targets you can run to get coverage report : 

```
make coverage_all_txt
OR 
make coverage_all_html
```

Respectively building a text summary ```coverage_all.txt``` and a detailed html report ```coverage_all.html``` in ```llvmcoverage``` directory. 



# Other 

Aside code, prefer text-based file formats over binaries for contributions, as for example SVG for graphics, as they are lighter to handle for git and keep repository light as well for people to download.