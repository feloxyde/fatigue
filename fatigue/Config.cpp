#include "Config.hpp"

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


void Config::loadFromOpts(cxxopts::ParseResult const& res)
{
  showParamNames = res["shownames"].as<bool>();
  showParamTypes = res["showtypes"].as<bool>();

  runner = res["runner"].as<std::string>();

  if (res["select"].count()) {
    filter.select = res["select"].as<std::string>();
  }

  if (res["exclude"].count()) {
    filter.exclude = res["exclude"].as<std::string>();
  }

  //listing runners
}

} // namespace ftg
