#include "Config.hpp"

#include "OstreamTestRunner.hpp"
#include <cxxopts.hpp>
#include <iostream>
#include <memory>
#include <regex>

namespace ftg {

Filter::Filter() : select(), exclude(), separator("//")
{
}
Filter::~Filter()
{
}

bool Filter::shouldRun(std::vector<std::string> const& prefixes, std::string const& test) const
{
  std::string fullname = "";

  for (auto const& s : prefixes) {
    fullname += s + separator;
  }

  fullname += separator + test;

  bool run = true;
  if (select.has_value()) {
    run = run && std::regex_match(fullname, select.value());
  }
  if (exclude.has_value()) {
    run = run && !std::regex_match(fullname, exclude.value());
  }
  return run;
}

Config::Config() : showParamNames(false), showParamTypes(false), runner("default"), filter()
{
}

Config::~Config()
{
}

void Config::loadFromCLI(int argc, char** argv)
{
  // clang-format off
  cxxopts::Options opts("Fatigue built test software", "You are supposed to know this btw.");
  opts.add_options()
  ("t,showtypes", "show parameter types when displaying checks results", cxxopts::value<bool>()->default_value("false"))
  ("n,shownames", "show parameter names when displaying checks results", cxxopts::value<bool>()->default_value("false"))
  ("r,runner","selects which runner to use to conduct tests", cxxopts::value<std::string>()->default_value("default"))
  ("s,select", "runs tests matching a regular expression", cxxopts::value<std::string>())
  ("e,exclude", "excludes tests matching a regular expression", cxxopts::value<std::string>());
  // clang-format on
  auto results = opts.parse(argc, argv);

  showParamNames = results["shownames"].as<bool>();
  showParamTypes = results["showtypes"].as<bool>();

  runner = results["runner"].as<std::string>();

  if (results["select"].count()) {
    filter.select = results["select"].as<std::string>();
  }

  if (results["exclude"].count()) {
    filter.exclude = results["exclude"].as<std::string>();
  }
}

} // namespace ftg
