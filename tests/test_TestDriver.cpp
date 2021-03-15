#include "fatigue/TestDriver.hpp"

#include <cassert>
#include <cstddef>

using namespace ftg;

int main(){
    size_t index = 0;

    TestDriver td("name");
    assert(td.passed());
    assert(td.log().size() == 0);

    //passing reporter
    index++;
    auto r1 = std::make_unique<CheckReporter>(td, "pass", true);
    r1.reset();

    assert(td.passed());
    assert(td.log().size() == 0);

    index++;
    auto r4 = std::make_unique<CheckReporter>(td, "warn fail", false);
    r4->warn();
    r4.reset();

    assert(td.passed());
    assert(td.log().size() == 1);

    LogMessage lm4 = td.log()[0];
    assert(!lm4.isImportant());
    assert(lm4.sindex() == index);
    assert(lm4.tindex() == index);
    assert(lm4.mode() == MESSAGE_WARN);
    assert(lm4.description() == "expected warn fail to succeed, but failed.");

    //failing reporter, default msg
    index++;
    auto r2= std::make_unique<CheckReporter>(td, "fail", false);
    r2.reset();

    assert(!td.passed());
    assert(td.log().size() == 2);

    LogMessage lm2 = td.log()[1];
    assert(!lm2.isImportant());
    assert(lm2.sindex() == index);
    assert(lm2.tindex() == index);
    assert(lm2.mode() == MESSAGE_CHECK);
    assert(lm2.description() == "expected fail to succeed, but failed.");
    
    //passing reporter, fatal
    index++;
    auto r3 = std::make_unique<CheckReporter>(td, "fatal pass", true);
    r3->fatal();
    r3.reset();

    assert(!td.passed());
    assert(td.log().size() == 2);

    //failing reporter, fatal
    index++;
    bool thrown = false;
    auto r5 = std::make_unique<CheckReporter>(td, "fatal fail", false);
    try{
        r5->fatal();
        r5.reset();
    } catch (FatalCheckFailure fe) {
        thrown = true;
    }
    assert(thrown);

    assert(!td.passed());
    assert(td.log().size() == 3);

    LogMessage lm5 = td.log()[2];
    assert(!lm5.isImportant());
    assert(lm5.sindex() == index);
    assert(lm5.tindex() == index);
    assert(lm5.mode() == MESSAGE_FATAL);
    assert(lm5.description() == "expected fatal fail to succeed, but failed.");

    //failing important
    index++;
    auto r6= std::make_unique<CheckReporter>(td, "important fail", false);
    r6->important();
    r6.reset();

    assert(!td.passed());
    assert(td.log().size() == 4);

    LogMessage lm6 = td.log()[3];
    assert(lm6.isImportant());
    assert(lm6.sindex() == index);
    assert(lm6.tindex() == index);
    assert(lm6.mode() == MESSAGE_CHECK);
    assert(lm6.description() == "expected important fail to succeed, but failed.");
   
    index++;
    auto r7= std::make_unique<CheckReporter>(td, "success", true);
    r7->fails();
    r7.reset();

    assert(!td.passed());
    assert(td.log().size() == 5);

    LogMessage lm7 = td.log()[4];
    assert(!lm7.isImportant());
    assert(lm7.sindex() == index);
    assert(lm7.tindex() == index);
    assert(lm7.mode() == MESSAGE_CHECK);
    assert(lm7.description() == "expected success to fail, but succeeded.");
   
    //failing important
    index++;
    auto r8 = std::make_unique<CheckReporter>(td, "important fail succeeds", false);
    r8->important().succeeds();
    r8.reset();

    assert(!td.passed());
    assert(td.log().size() == 6);

    LogMessage lm8 = td.log()[5];
    assert(lm8.isImportant());
    assert(lm8.sindex() == index);
    assert(lm8.tindex() == index);
    assert(lm8.mode() == MESSAGE_CHECK);
    assert(lm8.description() == "expected important fail succeeds to succeed, but failed.");
   
    return 0;
}