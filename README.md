<!--
Copyright 2021 Felix Bertoni

SPDX-License-Identifier: MIT
-->

# fatigue 

*Framework for Advanced Testing Inducing a Grotestque User Experience.* 
A simple C++ testing framework, template based and macro free. It is mainly oriented toward unit, integration testing through specification checking.

Please see documentation section for more informations. 

[[_TOC_]]

# Overview

This section is meant to give you a quick overview of how fatigue works, so you can decide quickly to adpot it or not. Please see documentation section for more informations regarding fatigue.

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
    check_near_equal(9, 9, 0.3, "some custom description");
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

The program returns ```0``` if all tests succeeds, something else if some tests fails. 

Checks can customized to display a special message

```cpp
//in run method
check_true(true);
check_equal(10, 20.0);
check_true(false, "custom check");
//end run method
```

Will produce output as : 

```
(2) [ERROR] check_equal( 10, 20 ) -> true : failed.
(3) [ERROR] custom check -> true : failed.
```
Some runners allows to display additional informations through cli switches, as names (```-n```) or types (```-t```) of checks arguments.

```
(2) [ERROR] expected check_equal( l: 10 [int), r: 20 [double] ) to succeed, but failed.
```

Please see documentation for a deeper insight.

# Planned features

The project is still in progress, and features will be added depending on my time and needs. This is an overview of some planned features, 
Please see issue list at [https://gitlab.com/feloxyde/fatigue/](https://gitlab.com/feloxyde/fatigue/)for more informations.

## Guaranteed features

- parallel and segfault proof test runners
- dependencies between tests
- superfast and lightweight runner

## On the fly features

- More checks than current available ones, especially regarding string and data collections
- Better display runners
- Test archetypes, as for example for testing ```==``` operator quickly.

# Documentation

Documentation of the project, including API reference, architecture, tutorials and more can be found at [https://feloxyde.gitlab.io/fatigue/html/index.html](https://feloxyde.gitlab.io/fatigue/html/index.html).

If for some reason you can't access documentation, you can build it with : 

- *Doxygen* and its dependencies, as Python.
- *dot*, a graphviz tool usually found in the graphviz package of your distribution.

Then, simply run ```doxygen Doxyfile``` at the root of directory and an HTML doc will be generated in 
```build/docgen``` directory, with ```build/docgen/html/index.html``` as entry point. 

# Contributions

Contributions are welcome, please read [Contributing.md](Contributing.md). 

This project has currently not received contributions aside from maintainer, Felix Bertoni, [felix.bertoni987@gmail.com](felix.bertoni987@gmail.com).

