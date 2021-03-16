#include "fatigue/Logger.hpp"
#include <fatigue/VectorLogger.hpp> 

#include <cassert>

using namespace ftg;


int main(){

    VectorGlobalLogger vgl;
    auto vsl = vgl.suiteLogger();
     
    vsl->name("suite1");

    auto vtl = vsl->testLogger();
    vtl->name("test1.1");
    vtl->message(MESSAGE_CHECK, "check1.1.1", true);
    vtl->checkPassed();
    vtl->message(MESSAGE_WARN, "warn.1.3", false);
    vtl->checkPassed();
    vtl->checkPassed();
    vtl->message(MESSAGE_FATAL, "fatal.1.6", false);
    vtl->commit();
    vtl.reset();

    auto vtl2 = vsl->testLogger();
    vtl2->name("test1.2");
    vtl2->checkPassed();
    vtl2->checkPassed();
    vtl2->checkPassed();
    vtl2->checkPassed();
    vtl2->commit();
    vtl2.reset();

    auto vtl3 = vsl->testLogger();
    vtl3->name("test1.3");
    vtl3->checkPassed();
    vtl3->message(MESSAGE_CHECK, "check1.2.1", false);
    vtl3->uncaughtException();

    vsl->commit();
    vsl.reset();

    auto vsl2 = vgl.suiteLogger();
    vsl2->name("suite2");
    
    auto vtl2_1 = vsl2->testLogger();
    vtl2_1->name("test2.1");
    vtl2_1->skipped();
    vtl2_1->commit();

    auto vtl2_2 = vsl2->testLogger();
    vtl2_2->name("test2.2");
    vtl2_2->loadFailure();
    vtl2_2->commit();

    vtl2_2.reset();
    vtl2_1.reset();

    vsl2->commit();
    vsl2.reset();

    vgl.commit();

    auto res = vgl.results();
    assert(res.size() == 2);

    //first suite 
    auto suite1 = res[0];
    assert(suite1.name == "suite1");

    assert(suite1.results.size() == 3);
    auto test1_1 = suite1.results[0];
    
    assert(test1_1.name == "test1.1");
    assert(test1_1.status == ftg::TEST_FINISHED);
    assert(test1_1.checkRan == 6);
    assert(test1_1.checkFailed == 3);
    
    assert(test1_1.messages.size() == 3);
    
    assert(test1_1.messages[0].description() == "check1.1.1");
    assert(test1_1.messages[0].sindex() == 1);
    assert(test1_1.messages[0].tindex() == 1);
    assert(test1_1.messages[0].isImportant());
    assert(test1_1.messages[0].mode() == MESSAGE_CHECK);

    assert(test1_1.messages[1].description() == "warn.1.3");
    assert(test1_1.messages[1].sindex() == 3);
    assert(test1_1.messages[1].tindex() == 3);
    assert(!test1_1.messages[1].isImportant());
    assert(test1_1.messages[1].mode() == MESSAGE_WARN);

    assert(test1_1.messages[2].description() == "fatal.1.6");
    assert(test1_1.messages[2].sindex() == 6);
    assert(test1_1.messages[2].tindex() == 6);
    assert(!test1_1.messages[2].isImportant());
    assert(test1_1.messages[2].mode() == MESSAGE_FATAL);

    auto test1_2 = suite1.results[1];
    
    assert(test1_2.name == "test1.2");
    assert(test1_2.status == ftg::TEST_FINISHED);
    assert(test1_2.checkRan == 4);
    assert(test1_2.checkFailed == 0);
    assert(test1_2.messages.size() == 0);

    auto test1_3 = suite1.results[2];
    
    assert(test1_3.name == "test1.2");
    assert(test1_3.status == ftg::TEST_EXCEPTION_EXIT);
    assert(test1_3.checkRan == 2);
    assert(test1_3.checkFailed == 1);
    assert(test1_3.exceptionSindex == 2);
    assert(test1_3.exceptionTindex == 2); 
    
    assert(test1_3.messages.size() == 1);
    assert(test1_3.messages[0].description() == "check.2.1");
    assert(test1_3.messages[0].sindex() == 2);
    assert(test1_3.messages[0].tindex() == 2);
    assert(!test1_3.messages[0].isImportant());
    assert(test1_3.messages[0].mode() == MESSAGE_CHECK);


    auto suite2 = res[0];
    assert(suite2.name == "suite2");

    assert(suite2.results.size() == 2);
    auto test2_1 = suite2.results[0];
    assert(test2_1.name == "test2.1");
    assert(test2_1.status == ftg::TEST_SKIPPED);
    
    auto test2_2 = suite2.results[1];
    assert(test2_2.name == "test2.2");
    assert(test2_2.status == ftg::TEST_LOAD_FAILURE);
}