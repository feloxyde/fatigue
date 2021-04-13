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
RESULT testJSONCopy(std::vector<Factory<T>> variations)
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

What interests me the most in testing frameworks, is the ease of driving tests and getting a clean report of a test run. 