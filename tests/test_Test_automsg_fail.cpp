#include "fatigue/TestDriver.hpp"
#include <fatigue/Test.hpp>


#include <cassert>

using namespace ftg;

struct TestAutoMsgFail : public Test {
    TestAutoMsgFail() : Test("TestAutoMsgFail"){}

    virtual void run(){
        check_equal(10, 9);
        
        check_near_equal(20, 12, 2);
        check_near_equal(7, 10, 2);
        
        check_not_equal(12, 12);
        
        check_less_than(12, 10);
        check_less_than(12, 12);

        check_less_equal(12, 10);
        
        check_greater_than(8, 9);
        check_greater_than(10, 10);
        
        check_greater_equal(7, 8);

        check_throw<int>([](){});

        check_nothrow([](){throw 1;});

        check_true(false);

        check_false(true);
    }

};

int main(){

    TestAutoMsgFail t;
    
    t.run();
    assert(!t.passed());
    
    assert(t.log().size() == 14);
    

    LogMessage lm = t.log()[0];
    assert(!lm.isImportant());
    assert(lm.sindex() == 1);
    assert(lm.tindex() == 1);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_equal with 10 and 9 to succeed, but failed.");

    lm = t.log()[1];
    assert(!lm.isImportant());
    assert(lm.sindex() == 2);
    assert(lm.tindex() == 2);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_near_equal with 20 and 12 tolerance 2 to succeed, but failed.");

    lm = t.log()[2];
    assert(!lm.isImportant());
    assert(lm.sindex() == 3);
    assert(lm.tindex() == 3);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_near_equal with 7 and 10 tolerance 2 to succeed, but failed.");
 
    lm = t.log()[3];
    assert(!lm.isImportant());
    assert(lm.sindex() == 4);
    assert(lm.tindex() == 4);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_not_equal with 12 and 12 to succeed, but failed.");
 
    lm = t.log()[4];
    assert(!lm.isImportant());
    assert(lm.sindex() == 5);
    assert(lm.tindex() == 5);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_less_than with 12 and 10 to succeed, but failed.");
 
    lm = t.log()[5];
    assert(!lm.isImportant());
    assert(lm.sindex() == 6);
    assert(lm.tindex() == 6);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_less_than with 12 and 12 to succeed, but failed.");
 
    lm = t.log()[6];
    assert(!lm.isImportant());
    assert(lm.sindex() == 7);
    assert(lm.tindex() == 7);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_less_equal with 12 and 10 to succeed, but failed.");
 
    lm = t.log()[7];
    assert(!lm.isImportant());
    assert(lm.sindex() == 8);
    assert(lm.tindex() == 8);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_greater_than with 8 and 9 to succeed, but failed.");

    lm = t.log()[8];
    assert(!lm.isImportant());
    assert(lm.sindex() == 9);
    assert(lm.tindex() == 9);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_greater_than with 10 and 10 to succeed, but failed.");
 
    lm = t.log()[9];
    assert(!lm.isImportant());
    assert(lm.sindex() == 10);
    assert(lm.tindex() == 10);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_greater_equal with 7 and 8 to succeed, but failed.");
 
    lm = t.log()[10];
    assert(!lm.isImportant());
    assert(lm.sindex() == 11);
    assert(lm.tindex() == 11);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_throw exception int to succeed, but failed.");
 
    lm = t.log()[11];
    assert(!lm.isImportant());
    assert(lm.sindex() == 12);
    assert(lm.tindex() == 12);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_nothrow to succeed, but failed.");
 
    lm = t.log()[12];
    assert(!lm.isImportant());
    assert(lm.sindex() == 13);
    assert(lm.tindex() == 13);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_true to succeed, but failed.");
 
    lm = t.log()[13];
    assert(!lm.isImportant());
    assert(lm.sindex() == 14);
    assert(lm.tindex() == 14);
    assert(lm.mode() == MESSAGE_CHECK);
    assert(lm.description() == "expected check_false to succeed, but failed.");
 

    return 0;
}