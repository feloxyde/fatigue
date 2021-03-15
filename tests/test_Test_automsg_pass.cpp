#include "fatigue/TestDriver.hpp"
#include <fatigue/Test.hpp>


#include <cassert>

using namespace ftg;

struct TestAutoMsgPass : public Test {
    TestAutoMsgPass() : Test("TestAutoMsgPass"){}

    virtual void run(){
        check_equal(10, 10);
        
        assert(passed());
        check_near_equal(10, 12, 2);
        assert(passed());
        check_near_equal(10, 10, 2);
        assert(passed());
        check_near_equal(12, 10, 2);
        assert(passed());
        
        check_not_equal(10, 12);
        assert(passed());
        
        check_less_than(8, 10);
        assert(passed());

        check_less_equal(8, 10);
        assert(passed());
        check_less_equal(10, 10);
        assert(passed());
        
        check_greater_than(10, 9);
        assert(passed());
        
        check_greater_equal(10, 8);
        assert(passed());
        check_greater_equal(10, 10);
        assert(passed());

        check_throw<int>([](){
            throw (int)1;
        });
        assert(passed());

        check_nothrow([](){});
        assert(passed());

        check_true(true);
        assert(passed());

        check_false(false);
        assert(passed());
    }

};

int main(){

    TestAutoMsgPass t;
    
    t.run();
    assert(t.passed());
    
    return 0;
}