#ifndef FATIGUE_CONFIG_HPP  
#define FATIGUE_CONFIG_HPP  


#include "fatigue/TestRunner.hpp"
#include <memory>
#include <cxxopts.hpp>
#include <regex>

namespace ftg {
//fixme need to use regex, check if a regex can be negated, and policy to combine multiple filter ???
//maybe just "run" and "excludes" regexes for both suite and test
class Config;

Config& config();

struct Config final {
    bool showParamNames;
    bool showParamTypes;
    std::unique_ptr<TestRunner> runner;

public:
    static Config& instance();
    bool filter(std::string const& suite, std::string const& test);
private:
    static std::unique_ptr<Config> instancePtr;



public:
    Config();
    ~Config();

    void loadFromCLI(int argc, char**argv);
};

}


#endif 