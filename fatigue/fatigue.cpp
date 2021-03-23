#include "fatigue.hpp"
#include "fatigue/TestRunner.hpp"

namespace ftg {

std::vector<std::unique_ptr<Suite>> FatigueDriver::suites = std::vector<std::unique_ptr<Suite>>();

    
FatigueDriver FatigueDriver::declare(std::unique_ptr<Suite> suite) const
{
    suites.push_back(std::move(suite));
    return FatigueDriver();
}



unsigned FatigueDriver::run() const{
    assert(config().runner);
    return config().runner->run(suites);
};

FatigueDriver fatigue(int argc, char**argv)
{
    config().loadFromCLI(argc, argv);
    return FatigueDriver();
}


}