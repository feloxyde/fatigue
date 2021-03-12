# itootuf

itoofut (I'm Tired Of Other Frameworks for Unit Testing), a simple and flexible testing framework. Probably not as user friendly as others, but more flexible and still simple

Note : May need to change namespace itoofut to itft or something like this ?

# Problem

## Flexibility

I was writing tests for another project, and somehow ended up with a need to test some assing-like methods on different types. Here is a quick example. 

```cpp

struct MyStruct {
    /*some constructors here */
    MyStruct(Mystruct const& origin);

    MyStruct(JSON const& json);
    operator JSON() const;

    MyStruct& operator=(MyStruct const& other);

    /* some more code */
private:
    /* some nontrivial copy members here,
    A, B and C 
    */
};
```

Construction and cast to ```JSON``` is meant to be directional, so all three following lines are strictly equivalents in term of object they produce :

```cpp
Mystruct a(...), b(...);
MyStruct a(b);
MyStruct c(b);
MyStruct d( JSON(b) );
```

My requirements here for tests are fairly simple : For each operation, I want to write a test suite that takes in an arbitrary number of ```MyStruct``` factories as parameter, and check that their result operation matches the original. It shall be compatible with other types as well. So, roughly, is would perform the same operations as following code.

```cpp
/* true if fails, false if passes */
template<typename T>
RESULT testAssignOperator(std::vector<Factory<T>> variations)
{
    for (auto const& v : variations){
        T origin = v.create();
        T target(); //default construction or something like that 
        target = origin;
        if(!(target == origin)){
            return FAIL; //of course with more info
        }
    }
    return PASS;
}
```
That's the idea, except that I want to get a more detailed report, and especially, I want to have *one test* for *each* variation. At this point, most, if not all, frameworks are already unable to provide a flexible solution. 

## Dependencies

When writing my tests, I often write them *assuming* some functionality of the subject or other components are passing already. For example, in my previous templated test function example, I assumed that ```bool T::operator==(T const& other) const``` is functional. 

On a quick sidenote, this operator is even more time-consuming to test than assign-like operators and methods, as it requires to check that *two of the same variation are matching* **and** *two elements of different variants are not matching*. For ```n``` variation, it requires performing roughly ```n^2``` assertions.


```cpp
/* true if fails, false if passes */
template<typename T>
RESULT testEqualOperator(std::vector<Factory<T>> variations)
{
    for(size_t v = 0; v < variations.size(); v++){
        T target = variations[v].create();
        for(size_t u = 0; u < variations.size(); u++){
            T sample = variations[u].create();
            if(v == u && !(target == sample) ||
               v != u && target == sample) {
                   return FAIL;
            }
        }
    }
    return TRUE;
}
```

It becomes even more a nightmare to handle with most frameworks if you want precise report. But that's not the point here, let's back on topic.

So, the idea is that I want that in case tests for ```operator=``` and ```operator==``` both fails, I want to be informed that the first depends on the second. This way, I know that it would be wise to first investigate why the second didn't pass. 
I also, eventually, want a way to tell that no test were run for this particular operator, as *test for MyStruct::operator= depends on MyStruct::operator==, which doesn't appears to have been tested.*

## Learning curve, debug and portability.

Most frameworks able to handle some of previously regarded features are :
1) Using macros extensively, making it hard to debug certain errors, as well as increasing the difficulty of making hacks.
2) Are not using C++ 20 concepts, making it hard to debug when they rely on templates.
3) Are not using std library only, and thus adding even more dependencies to the project. 
4) Are not header only, which can make them nontrivial to install on unusual systems.  
5) Most frameworks are unable to display conditional messages to give more info if test fails, which is quite inconvenient. 


# Solution : Three class Framework

In order to use the itoofut framework, only three classes, fairly fast, are to be known. It's use are somehow inspired from Pytest framework, where tests are implemented by deriving a test class. 

Especially, this framework is oriented toward ensuring debug is as simple as possible.

## ```Test```

Is the struct holding a test. Tests shall be derived from it. 
In order to run it, you only need to override four methods, each taking no arguments. Please note ```Test``` takes a mandatory ```std::string``` name parameter.

- ```run```, mandatory, which runs the test
- ```load```, optional, which creates resources needed for the test.
- ```unload```, optional, which destroys resources needed for the test.
- ```depends```, optional, which returns a list of tests identifiers that the test depends on.

```cpp 
//should Test should be instanciated with final class, in order to auto name ?
//Test has an additional
struct MyTest : public itoofut::Test {
    virtual void load();
    virtual void run();
    virtual void unload();
    virtual std::list<TestId> name() const; 
};
```

The test class provides utilities to write checks in your tests. 

```cpp
check_equal(a, b); /* ensures that A and B are equal */
check_close(a, b, tolerance); /* ensures that A and B are near equal */
/* ... */
```
By default, checks are non-fatal, and test will continue. By default, a failing assertion will fail the test. 
Check can stop test by specifying ```FATAL``` as a template parameter, or by using asserts.

```cpp
check_equal<FATAL>(a, b);
assert_equal(a, b); //equivalent to previous line
```

Check can print a message without stopping the test by specifying ```WARN``` as a template parameter, or using warnings.

```cpp
check_equal<WARN>(a, b);
warn_equal(a, b); //equivalent to previous line
```

Checks can take an optional string message parameter to give precision on what failed. 

```cpp
check_equal<WARN>(a, b, "a and b not equal feels strange, please rerun test");
```

Finally, the class gives options to display messages. Those messages are displayed *only if the test fails*. By default, they are displayed if either the previous or the next check fails. This behavior can be changed by setting template parameters. 

```cpp 
assert_equal(a, b); //1
message("a = " + a); //this will be displayed either assert 1 or 2 fails
assert_equal(a, c); //2
```

```cpp
assert_equal(a, b); //1
assert_equal(a, c); //2
message<PREV, 2>("a = "); //this will be displayed 
                          //if either assert 1 or 2 fails, but not if 3 fails
assert_equal(a, c); //3
```

```cpp
assert_equal(a, b); //1
message<NEXT, 2>("a = "); //this will be displayed if either assert 
                          //2 or 3 fails, but not if 1 fails
assert_equal(a, c); //2
assert_equal(a, c); //3
```

```cpp
assert_equal(a, b); //1
message<ANY, 2>("a = "); //this will be displayed if either 
                         //assert 1, 2 or 3 fails
assert_equal(a, c); //2
assert_equal(a, c); //3
```

```cpp
assert_equal(a, b); //1
message<TEST>("a = "); //this will be displayed if test fails.
assert_equal(a, c); //2
assert_equal(a, c); //3
```

## ```TestSuite```

Test suite provides a way to generate a battery of tests and run them in isolated behavior. **Please note that for now, isolation is only in the same thread, but it could later be in a different process!**. 

To create your own suite, simply derive from ```TestSuite``` struct, and override the ```testsList tests() const``` method. 
Please note that ```TestSuite``` constructor takes a ```std::string``` name as mandatory parameter.

```cpp 
struct MySuite : public itoofut::TestSuite {
    /* some ctor */
    virtual void testList tests() const;
}
```

```testList``` is an alias for 

```cpp
std::unique_ptr<std::vector<std::unique_ptr<itoofut::Test>>>;
```

With this, we can handle our nasty test cases. 

## ```TestRunner```

Test runner provides a singleton interface that is able to register suites, as well as a command line parser for filtering tests and choosing which one to run. 
You can interact with it either with 

Usually, the bottom of the *main* file of your tests will look like that : 

```cpp
void register_tests(){
    ::itoofut::add(std::make_unique<MySuite>("MySuite"));
    ::itoofut::add(std::make_unique<AnotherSuite>("AnotherSuite"));
    /* ... */
}

int main(int argc, char** argv){
    register_tests();
    ::itoofut::run(argc, argv);
}
```

FIXME add syntax for cli here !

## Utilities

A function to convert a type into a string.