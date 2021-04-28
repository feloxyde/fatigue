#ifndef FATIGUE_CONFIG_HPP
#define FATIGUE_CONFIG_HPP

#include "TestRunner.hpp"
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
  bool shouldRun(std::vector<std::string> const& prefixes, std::string const& test) const;
};

struct Config final {

  Config();
  ~Config();
  void loadFromCLI(int argc, char** argv);

public:
  bool showParamNames;
  bool showParamTypes;
  std::string runner;
  Filter filter;
};

} // namespace ftg

#endif