#include "Config.hpp"

#include <iostream>
#include <memory>
#include <cxxopts.hpp>
#include <regex>
#include "OstreamTestRunner.hpp"

namespace ftg {



Filter::Filter() : select(), exclude(), separator("//"){}
Filter::~Filter(){}

bool Filter::shouldRun(std::string const& suite, std::string const& test) const
{
    std::string fullname=suite + separator + test;

    bool run = true;
    if(select.has_value()){
        run = std::regex_match(fullname, select.value());
    }
    if(exclude.has_value()){
        run = ! std::regex_match(fullname, exclude.value());
    }

    return run;
}



Config& config() {
    return Config::instance();
}
//#FIXME note : can use some kind of defer caller to actually force configure/add tests/run in the right order : 
//ftg::configure(argc, argv).addSuite().addSuite().[...].run();

Config& Config::instance() {
    if(!instancePtr){
        instancePtr = std::make_unique<Config>(); 
    }
    return *instancePtr;
}

std::unique_ptr<Config> Config::instancePtr;

Config::Config() : showParamNames(false), showParamTypes(false), runner(std::make_unique<OstreamTestRunner>(std::cout)), filter(){}
Config::~Config(){}

void Config::loadFromCLI(int argc, char**argv)
{
    cxxopts::Options opts("Fatigue built test software", "You are supposed to know this btw.");
    opts.add_options()
    ("t,showtypes", "show parameter types when displaying checks results", cxxopts::value<bool>()->default_value("false"))
    ("n,shownames", "show parameter names when displaying checks results", cxxopts::value<bool>()->default_value("false"))
    ("r,runner", "selects which runner to use to conduct tests", cxxopts::value<std::string>()->default_value("SequentialCout"));

    auto results = opts.parse(argc, argv);

    showParamNames = results["shownames"].as<bool>();
    showParamTypes = results["showtypes"].as<bool>();
    
    if(results["runner"].as<std::string>() == "SequentialCout"){
        runner = std::make_unique<OstreamTestRunner>(std::cout);
    } 
   
}

}
