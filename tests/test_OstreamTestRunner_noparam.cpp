#include "fatigue/Suite.hpp"
#include <fatigue/OstreamTestRunner.hpp>
#include <fatigue/TestDriver.hpp>
#include <fatigue/TestRunner.hpp>
#include <fatigue/Test.hpp>
#include <memory>
#include <sstream>
#include <cassert>
#include <string>


using namespace ftg;

template<int num, bool pass>
struct MockTest : public ftg::Test {
    MockTest():Test(std::string("MockTest") + std::to_string(num) + ":" + std::to_string(pass)){

    }
    virtual ~MockTest(){}
    virtual void run(){
        check_true(pass);
    }

};

struct Suite1 : ftg::Suite {
    Suite1 () : Suite("suite1"){}
    virtual ~Suite1(){}

    virtual TestList tests() const {
        TestList tl;
        tl.push_back(std::move(std::make_unique<MockTest<1, false>>()));
        tl.push_back(std::move(std::make_unique<MockTest<4, true>>()));

        return tl;
    }
};

struct Suite2 : ftg::Suite {
    Suite2 () : Suite("suite2"){}
    virtual ~Suite2(){}

    virtual TestList tests() const {
        TestList tl;
        tl.push_back(std::move(std::make_unique<MockTest<10, true>>()));
        tl.push_back(std::move(std::make_unique<MockTest<13, true>>()));
        tl.push_back(std::move(std::make_unique<MockTest<14, true>>()));
        return tl;
    }
};



int main(){

    std::vector<std::unique_ptr<Suite>> suites;
    suites.push_back(std::make_unique<Suite1>());
    
    std::stringstream ss;
    std::stringstream res;

    OstreamTestRunner otr(ss);
    otr.run(suites);
    
    

    return 0;
}