#include <fatigue/Config.cpp>
#include <cassert>


using namespace ftg;

int main(){

    Filter f;
    assert(!f.select.has_value());
    assert(!f.exclude.has_value());
    assert(f.separator == "//");
    assert(f.shouldRun("some, name", "some other"));
    assert(f.shouldRun("XXX", "XXXXXXX"));


    Filter fs;
    fs.select = ".*test.*";
    assert(fs.shouldRun("somesuite", "sometestname"));
    assert(!fs.shouldRun("somesuite", "somename"));

    Filter fe;
    fe.exclude = ".*test.*";
    assert(!fe.shouldRun("somesuite", "sometestname"));
    assert(fe.shouldRun("somesuite", "somename"));


    Filter fes;
    fes.select = ".*test.*";
    fes.exclude = ".*name.*";
    assert(fes.shouldRun("somesuite", "sometest"));
    assert(!fes.shouldRun("somesuite", "sometestname"));
    assert(!fes.shouldRun("somesuite", "somename"));

    Filter fsep;
    fsep.select = "suite.*//test";
    assert(fsep.shouldRun("suite", "test"));
    assert(!fsep.shouldRun("tsuite", "suite"));
    assert(!fsep.shouldRun("suite", "sometest"));
    assert(fsep.shouldRun("suite222", "test"));

    Filter fsep2;
    fsep2.separator = "--";
    fsep2.select = "suite.*--test";
    assert(fsep2.shouldRun("suite", "test"));
    assert(!fsep2.shouldRun("tsuite", "suite"));
    assert(!fsep2.shouldRun("suite", "sometest"));
    assert(fsep2.shouldRun("suite222", "test"));


    return 0;
};