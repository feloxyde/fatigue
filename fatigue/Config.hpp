#ifndef FATIGUE_CONFIG_HPP
#define FATIGUE_CONFIG_HPP

#include "fatigue/TestRunner.hpp"
#include <cxxopts.hpp>
#include <memory>
#include <regex>

namespace ftg {
//fixme need to use regex, check if a regex can be negated, and policy to combine multiple filter ???
//maybe just "run" and "excludes" regexes for both suite and test

struct Filter final {
  Filter();
  ~Filter();
  std::optional<std::regex> select;
  std::optional<std::regex> exclude;
  std::string separator;
  bool shouldRun(std::string const& suite, std::string const& test) const;
};
class Config;

Config& config();

struct Config final {
  bool showParamNames;
  bool showParamTypes;
  std::unique_ptr<TestRunner> runner;
  Filter filter;

public:
  static Config& instance();

private:
  static std::unique_ptr<Config> instancePtr;

public:
  Config();
  ~Config();

  void loadFromCLI(int argc, char** argv);
};

} // namespace ftg

#endif