#include "fatigue/TestDriver.hpp"
#include "fatigue/TestRunner.hpp"
#include <fatigue/OstreamTestRunner.hpp>
#include <fatigue/Test.hpp>
#include <sstream>

void ssr(std::stringstream& ss1, std::stringstream& ss2){
    ss1.clear();
    ss1.str(std::string());
    ss2.clear();
    ss2.str(std::string());
}


using namespace ftg;

int main(){
    size_t index = 0;

    TestDriver td("name");
    std::stringstream ss;
    std::stringstream res;
    OstreamTestLogger otl(ss);

    td.setLogger(&otl);
    assert(otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 0);
    assert(ss.str() == res.str());

    //passing reporter
    auto r1 = std::make_unique<CheckReporter>(td, "pass", std::vector<ParamInfo>(), true);
    r1.reset();
    assert(otl.m_checkPassed == 1);
    assert(otl.m_checkFailed == 0);
    assert(ss.str() == res.str());

    auto r4 = std::make_unique<CheckReporter>(td, "warn fail", std::vector<ParamInfo>(), false);
    r4->warn();
    r4.reset();
    assert(otl.passed());
    assert(otl.m_checkPassed == 1);
    assert(otl.m_checkFailed == 1);
    res << "(2) [WARN] expected warn fail to succeed, but failed." << std::endl;
    std::cout << res.str() << std::endl;
    std::cout << ss.str() << std::endl;
    assert(ss.str() == res.str());

    //failing reporter, default msg
    auto r2= std::make_unique<CheckReporter>(td, "fail", std::vector<ParamInfo>(), false);
    r2.reset();    
    assert(!otl.passed());
    assert(otl.m_checkPassed == 1);
    assert(otl.m_checkFailed == 2);
    res << "(3) [ERROR] expected fail to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());
    
    //passing reporter, fatal
    auto r3 = std::make_unique<CheckReporter>(td, "fatal pass", std::vector<ParamInfo>(), true);
    r3->fatal();
    r3.reset();
    assert(!otl.passed());
    assert(otl.m_checkPassed == 2);
    assert(otl.m_checkFailed == 2);
    assert(ss.str() == res.str());

    //failing reporter, fatal
    bool thrown = false;
    auto r5 = std::make_unique<CheckReporter>(td, "fatal fail", std::vector<ParamInfo>(), false);
    try{
        r5->fatal();
        r5.reset();
    } catch (FatalCheckFailure fe) {
        thrown = true;
    }
    assert(thrown);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 2);
    assert(otl.m_checkFailed == 3);
    res << "(5) [FATAL] expected fatal fail to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    //failing important
    auto r6= std::make_unique<CheckReporter>(td, "important fail", std::vector<ParamInfo>(), false);
    r6->important();
    r6.reset();
    
    assert(!otl.passed());
    assert(otl.m_checkPassed == 2);
    assert(otl.m_checkFailed == 4 );
    res << "!!! (6) [ERROR] expected important fail to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());
   
    auto r7= std::make_unique<CheckReporter>(td, "success", std::vector<ParamInfo>(), true);
    r7->fails();
    r7.reset();

    assert(!otl.passed());
    assert(otl.m_checkPassed == 2);
    assert(otl.m_checkFailed == 5);
    res << "(7) [ERROR] expected success to fail, but succeeded." << std::endl;
    assert(ss.str() == res.str());

   
    //failing important
    auto r8 = std::make_unique<CheckReporter>(td, "important fail succeeds", std::vector<ParamInfo>(), false);
    r8->important().succeeds();
    r8.reset();

    assert(!otl.passed());
    assert(otl.m_checkPassed == 2);
    assert(otl.m_checkFailed == 6);
    res << "!!! (8) [ERROR] expected important fail succeeds to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());
   
    return 0;
}