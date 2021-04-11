# fatigue 

*Framework for Advanced Testing Inducing a Grotestque User Experience.* 
A simple C++ testing framework, template based and macro free. It is mainly oriented toward unit, integration testing through specification checking.

This document presents the framework quickly, and provides links to additional resources in case you want to learn more, would it be for using it or developing. 

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

There are switches to tell runner to display names or types (```-t```) of checks arguments (```-n```). 

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

# Installing

**to be completed**

## Manual build and install

**to be completed**

# Using fatigue

(here add an example)

# Contributing

## Dependencies 

(clang etc... for coverage)

**to be completed**

# Other

**to be completed**

## Inspirations and other tests frameworks
UnitTest++, pyunit


## Story of the project

**to be completed**
