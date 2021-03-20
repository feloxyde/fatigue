#ifndef FATIGUE_CONFIG_HPP  
#define FATIGUE_CONFIG_HPP  


#include <memory>
#include <cxxopts.hpp>

namespace ftg {


struct Config final {
    bool showParamNames;
    bool showParamTypes;


public:
    static Config& instance() {
        return inst;
    }
private:
    static Config inst;
public:
    Config() : showParamNames(false), showParamTypes(false){}
    ~Config();

    void loadFromCLI(int argc, char**argv)
    {
        cxxopts::Options opts("Fatigue built test software", "You are supposed to know this btw.");
        opts.add_options()
        ("t,showtypes", "show parameter types when displaying checks results", cxxopts::value<bool>()->default_value("false"))
        ("n,shownames", "show parameter names when displaying checks results", cxxopts::value<bool>()->default_value("false"));

        auto results = opts.parse(argc, argv);

        showParamNames = results["shownames"].as<bool>();
        showParamTypes = results["showtypes"].as<bool>();
    }
};

inline Config& config() {
    return Config::instance();
}

}


#endif 