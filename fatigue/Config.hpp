#ifndef FATIGUE_CONFIG_HPP  
#define FATIGUE_CONFIG_HPP  


#include <memory>
#include <cxxopts.hpp>

namespace ftg {

class Config;

Config& config();

struct Config final {
    bool showParamNames;
    bool showParamTypes;

public:
    static Config& instance();
private:
    static std::unique_ptr<Config> instancePtr;
public:
    Config();
    ~Config();

    void loadFromCLI(int argc, char**argv);
};


}


#endif 