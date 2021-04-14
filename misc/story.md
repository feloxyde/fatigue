# Fatigue's story

This is the (boring) story behind the project.

## At the beginning

I was writing tests for some other project, and somehow ended up with the need to test some ```operator=```-like or ```operator==```-like methods on different types. Here is a quick example.

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
Cast from ```MyStruct``` to ```JSON``` and vice versa is meant to allow interchange, so it works exactly like a copy construction : 

```cpp
MyStruct a();

MyStruct b(a);
MyStruct c(JSON(a));

//b and c hold the same value
assert(b == c);
```

As it is mentioned in listing before, copy and JSON handling are nontrivial for a lot of types, which are structs containing variants. 

What came in my mind when needing to test that was simply : I want to write a test that takes in an arbitrary number of ```MyStruct``` as parameters, and check that the result operation of *JSON copy* matches the original. Kind of following code.

```cpp
template<typename T>
RESULT testJSONInterchange(std::vector<Factory<T>> variations)
{
    for (auto const& v : variations){
        T origin = v.create();
        T target(JSON(v));
        if(!(target == origin)){
            return FAIL; //of course with more info
        }
    }
    return PASS;
}
```

That's the idea. At this point I was using UnitTest++, which is a lightweight test framework doing a quite nice job for my basic purposes. But, as it is really simple and macro based, it would be quite difficult to create such flexible test with a decent error reporting.

## To the concept and name

And so, I decided it was time to change framework, eventually moving onto a more complete one. Some of the frameworks I came accross were : 

- CppUnit : port of JUnit, pretty amazing but extra big, with dozens of classes. 
- GTest : Google's testing framework, quite big as well, and not handling templated test really well, mixing somehow macros and *real* code.
- TUT : A modern and lightweight testing framework, but using some strange template trick to auto register tests. 

All of those are nice frameworks, but I didn't felt like they were suiting my needs. Instead, I felt like they were *too restricting*, either by their size and complexity, or by their test registration. 

I began to be tired. Tired of looking for a test framework. I thought for myself *anyway, I will write thousands of lines of fixtures and mocks, why would I need an auto-registration mechanism that would save me few lines ?* 
And then I decided to build my own testing framework. A framework actually not framing much, which would have a really simplistic core, extensible with independent features, as specialized test cases templates. To remember the reason, I decided that *fatigue*, meaning *tiredness* in french, would be a good name. 

## Goal features

What interests me the most in testing frameworks, is the ease of driving tests and getting a clean report of a test run. This is usually what takes most time if you want to create tests from scratch. 

I like to see testing as some sort of mathematical proof : 
because some component is tested and working, I can use it to test other components. Being able to express dependencies between tests is something I long for, but it will probably be a late feature.
For example, in previous ```JSON``` interchange test,

```cpp
template<typename T>
RESULT testJSONInterchange(std::vector<Factory<T>> variations)
{
    for (auto const& v : variations){
        T origin = v.create();
        T target(JSON(v));
        if(!(target == origin)){
            return FAIL; //of course with more info
        }
    }
    return PASS;
}
```
we used ```bool T::operator==(T const& origin)``` to check the result. Which means that the result of ```testJSONInterchange``` is meaningless if ```operator==``` isn't functional. I want my framework to allow expressing such dependencies, so I get warned if either tests for ```operator==``` do not exist, or if some are failing.

Additionally, since testing revolves heavily around templates, I also want the testing framework to ouput clean messages in case of template error, which is trivial with C++20 *concepts*.

Finally, I initially wanted the framework to be header-only, hence allowing for great portability and ease of use, but it turns up quite difficult to implement in a clean way without having some implementation files to avoid mutual inclusion, so for now I decided to keep it as a compiled library.