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

template<int num, bool loadsuccess>
struct MockTest : public ftg::Test {
    MockTest():Test(std::string("MockTest") + std::to_string(num) + ":" + std::to_string(loadsuccess)){

    }
    virtual ~MockTest(){}

    virtual bool load() noexcept {
        return loadsuccess;
    }

    virtual void run(){
        check_true(true);
    }
};

struct Suite1 : ftg::Suite {
    Suite1 () : Suite("suite1"){}
    virtual ~Suite1(){}

    virtual TestList tests() const {
        TestList tl;
        tl.push_back(std::move(std::make_unique<MockTest<1, true>>()));
        tl.push_back(std::move(std::make_unique<MockTest<4, false>>()));

        return tl;
    }
};




int main(){

    std::vector<std::unique_ptr<Suite>> suites;
    suites.push_back(std::make_unique<Suite1>());
    
    std::stringstream ss;
    std::stringstream res;

    OstreamTestRunner otr(ss);
    unsigned f = otr.run(suites);
    assert(f == 1);
    

    res << "---------------------------" << std::endl;
    res << "------ RUNNING TESTS ------" << std::endl;
    res << "---------------------------" << std::endl;
    res << std::endl;
    res << std::endl;
    res << "##### suite1 #####" << std::endl;
    res << std::endl;
    res << "-- MockTest1:1 --" << std::endl;
    res << "-- passed : out of 1 checks, 0 failed. --" << std::endl;
    res << std::endl;
    res << "-- MockTest4:0 --" << std::endl;
    res << "-- failed : error during load phase --" << std::endl;
    res << std::endl;
    res << std::endl;
    res << "---------------------------" << std::endl;
    res << "---------- FAILED ---------" << std::endl;
    res << "ran : 2" << std::endl;
    res << "failed : 1" << std::endl;
    res << "---------------------------" << std::endl;
    /* HERE TO HELP DEBUG */
    size_t i(0);
    while(i < res.str().size() && i < ss.str().size()){
        if(res.str()[i] != ss.str()[i]){
            std::cout << "wong char at "<< i << std::endl;
            std::cout << "found" << res.str()[i] << " vs " << ss.str()[i] << std::endl;
            break;
        }
        i++;
    }
    

    std::cout << res.str() << std::endl;
    std::cout << ss.str() << std::endl;
    
    assert(res.str() == ss.str());

    return 0;
}