/** @file */

#ifndef FATIGUE_CONFIG_HPP
#define FATIGUE_CONFIG_HPP

#include "Runner.hpp"
#include "fatigue/external/cxxopts.hpp"
#include <memory>
#include <regex>

namespace ftg {
//fixme need to use regex, check if a regex can be negated, and policy to combine multiple filter ???
//maybe just "run" and "excludes" regexes for both suite and test

/** @brief Filtering which tests to run 
  
  When filtered, tests names and contained suites names are concatenated using separator.
  Then, the obtained string is matched against two (optional) regex. 
  First one, select, enables tests to run if their string matches. If empty, all are selected.
  Second one, exclude, enables tests from remaining set if their string doesn't match. If empty, all remaining are selected.
*/
struct Filter final {
  Filter();
  ~Filter();
  std::optional<std::regex> select;
  std::optional<std::regex> exclude;
  std::string separator;
  bool shouldRun(std::vector<std::string> const& prefixes, std::string const& test) const;
};

/** @brief Configuration used by fatigue and runners to change their behavior. */
struct Config final {

  Config();
  ~Config();
  void loadFromOpts(cxxopts::ParseResult const& res);

public:
  bool showParamNames;
  bool showParamTypes;
  std::string runner;
  Filter filter;
};

} // namespace ftg

#endif