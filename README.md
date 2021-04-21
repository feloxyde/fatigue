# fatigue 

*Framework for Advanced Testing Inducing a Grotestque User Experience.* 
A simple C++ testing framework, template based and macro free. It is mainly oriented toward unit, integration testing through specification checking.

This document presents the framework quickly, and provides links to additional resources in case you want to learn more, would it be for using it or developing. 

[[_TOC_]]

# Overview

## Minimal test setup

To create a test, simply derive from the ```Test``` class and define the ```virtual void run()``` method.

```cpp 
class MyTest : public ftg::Test {
    MyTest() : Test("MyTest"){}

    virtual void run() {
        
    }

};
```
Additionally, ```virtual void load() noexcept``` and ```virtual void unload() noexcept``` methods can be defined and will be called respectively just before and just after running the test through method ```run```. They are to be used for difficult or heavy ressource management. 

```Test``` class provides several checks, to call from the ```run``` method body to implement tests.

```cpp 
virtual void run() {
    check_equal(10, 20);
    check_true(false);
    check_near_equal(9, 9, 0.3, "some custom message");
    /* and a lot more to discover*/
}
```

Tests are to be grouped through test suites, by deriving from ```Suite``` class, and defining the ```virtual TestList tests()``` method.

```cpp
class MySuite : public ftg::Suite {
    MySuite() : Suite("MySuite"){}

    virtual ftg::TestList tests(){
        ftg::TestList tl;
        tl.push_back(std::make_unique<MyTest>());
        return tl;
    }
};
```

Finally, in the main of the test software, suites are registered and run according to CLI config.

```cpp
int main(int argc, char**argv){
    return fatigue(argc, argv)
    .declare(std::make_unique<MySuite>())
    .run();
}
```

The program returns ```0``` if all tests succeeds, ```1``` if some tests fails.

## Checks and reporting

Checks are implemented using template and C++20 concepts, allowing for clean error messages in case some requirements are not met. They can take two forms : 

```cpp
//reporting with default message, which is generally the name of the function plus parameters
check_xxx(value1, value2); 
//reporting with custom message
check_xxx(value1, value2, "custom message");  
```

Checks report three informations for each argument : name, value and type. 

Values are converted to strings using template function

```template<typename T> std::string ftg::to_string(T const& value);```

By default, it tries to use ```operator<<``` to push value into an ```std::ostream```, but you can override its behavior by specializing the template, in case you need a special output. 


Checks can have special flags set, through the use of method chaining.

```cpp
//Mark a check as important for it to be emphased in the report
check_equal(a, b).important(); 

//Mark a check as warning, failure will still be reported but 
//will not make test run fail
check_equal(a, b, "custom msg").warn(); 

//Mark check as fatal, in case of failure the run is aborted.
check_equal(a, b).fatal();

//important can be combined with warn or fatal : 
check_equal(a, b, "custom msg").important().fatal();

//fails tells that the check result shall be false
//following check will trigger and error if a==b
check_equal(a, b).fails();
```

## CLI and display

Currently, there is only one runner available, which executes tests sequentially. It is currently NOT segfault-proof, and one segfault will result into the crash of the entire test software. A more advanced runner is planned, but default one should be sufficient for most cases, as usually, segfaults are critical errors you want to solve first. Even if it crashes, it will report which test run caused segfault.

Default runner supports running certain tests based on regex matching on the name of tests and suites. 

```
./mytests --select "^MySuite//.*" --exclude ".*//Integer.*"
```

Runs all tests belonging to ```MySuite```, while excluding every test which name begins by ```Integer```.

As we don't use macros, it's fairly difficult to know the exact line and file from which the error originates. Failed checks are reported by their index in the test run. For example, 

```cpp
//in run method
check_true(true);
check_equal(10, 20.0);
//end run method
```

will produce :
```
(2) [ERROR] expected check_equal( 10, 20 ) to succeed, but failed.
```
because second check failed.

There are switches to tell runner to display names (```-n```) or types (```-t```) of checks arguments . 

```
(2) [ERROR] expected check_equal( l: 10 [int), r: 20 [double] ) to succeed, but failed.
```

## Planned features


The project is still in progress, and features will be added depending on my time and needs. 

### Guaranteed features

- parallel and segfault proof tests
- dependencies between tests

### On the fly features

- More checks than current available ones, especially regarding string and data collections
- Better display runners
- Test archetypes, as for example for testing ```==``` operator quickly.

# Installing

## Dependencies

To build the project for installation, you will need : 
- **CMake**.
- **Make** or any other build system, as **Ninja**.
- C++20-ready compiler.
- **Python 3**, if you want to run tests.
- **Git**, for submodules handling.
- **[cxxopts](https://github.com/jarro2783/cxxopts)** library, that you can either install from package manager or build and install from Gihub, which should be fairly fast since library is header-only.

## Linux

Once dependencies are installed, clone the repository. 

```
git clone https://gitlab.com/feloxyde/fatigue
```

Change into the directory and run update submodules. 

```
cd fatigue
git submodule init --recursive 
```

Then, create a ```build``` directory, change into it and call CMake to build target ```fatigue```.

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target fatigue
```

Install using CMake :

```
sudo cmake --build . --target install
```

Fatigue should now be installed on your computer, likely under ```/usr/local/lib``` and ```/usr/local/include```.

Optionally, you can run tests with installed version, by changing into ```tests/install```, and then running CMake there, to build and run tests with installed
library. You will need Python3 to do that.

```
cd ../tests/install
mkdir build
cd build
cmake .. 
cmake --build .
cmake --build . --target test
```

## Windows

Honestly, no idea, will be added and enforced later. Contributions are welcome.

## MacOS

Honestly, no idea, will be added and enforced later. Contributions are welcome.

# Using fatigue

(here add an example)

## Example sources

```cpp 
//file StructToTest.hpp
#include <string>

/* a simple class that says something about an integer */
struct StructToTest {
    int itos123(std::string s){
        if(s == "1"){
            return 1;
        } else if (s == "2"){
            return 2;
        } else if (s == "3"){
            return 3;
        } else {
            return 0;
        }
    }
}
```

```cpp
//file TestStruct.hpp
#include <fatigue/Test.hpp>
#include <fatigue/Suite.hpp>
#include <StructToTest.hpp>

struct TestOkValues : public ftg::Test {
    TestOkValues() : ftg::Test("TestOkValues"){}

    void run() {
        StructToTest stt;
        check_equal(stt.itos123("1"), 1);
        check_equal(stt.itos123("2"), 2, "itos(2) should be returning 2");
        check_equal(stt.itos123("3"), 3);
    }
}

struct TestNokValues : public ftg::Test {

    TestNokValues() : ftg::Test("TestOkValues"){}

    void run() {
        StructToTest stt;
        check_equal(stt.itos123("0"), 0);
        check_equal(stt.itos123("1200"), 0);
        check_equal(stt.itos123("-273"), 0);
        check_equal(stt.itos123("-2222"), 0);
    }
}

struct STTSuite : public ftg::Suite {
    STTSuite() : ftg::Suite("STTSuite"){}

    TestList tests() {
        TestList tl;
        tl.push_back(std::make_unique<TestOkValues>());
        tl.push_back(std::make_unique<TestNokValues>());
        return tl;
    }
}
```

```cpp
//file TestMain.cpp
#include "TestStruct.hpp"
#include "fatigue/fatigue.hpp"

int main(int argc, char** argv){
    return fatigue(argc, argv)
        .declare(std::make_unique<STTSuite>())
        .run();
}
```

## CMake build example

We can now build our tests by linking them with fatigue library.

```cmake
add_executable(TestMain TestMain.cpp)
target_link_libraries(TestMain fatigue)
add_test(TestMain COMMAND TestMain)
```

You can also use *cmake_llvm_coverage* (FIXME add link).

# Uninstalling 

## Linux 

To uninstall fatigue, you will have to manually remove files created by installation.

- ```libfatigue.*```, probably found in ```/usr/local/lib```.
- ```fatigue/``` directory, containing headers, and probably found in ```/usr/local/include```.

## Windows

Honestly, no idea, will be added and enforced later. Contributions are welcome.

## MacOS

Honestly, no idea, will be added and enforced later. Contributions are welcome.

# Contributions

Contributions are welcome, please read [Contributing.md](Contributing.md). 

This project has currently not received contributions aside from maintainer.

# Repository structure

- ```fatigue``` directory contains all sources of the project, including headers.
- ```tests``` directory is obviously for tests :
    - ```install``` is an additional, trivial CMake project to build tests when checking installation.
    - ```system``` are CLI-based tests.
    - ```unit``` are unit tests, which are actually more like integration tests somehow.
- ```cmake_llvm_coverage``` is a submodule containing cmake utility for test and coverage.
- ```docs``` contains docs and doxygen configs.
- ```misc``` contains various stuff, as logo, story, ...

# Misc

Project is currently developed by Felix Bertoni, [felix.bertoni987@gmail.com)](felix.bertoni987@gmail.com)

