// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

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

  showParamNames = res[RunnerOptions::shownames].as<bool>();
  if (showParamNames) {
    m_runnerOptions.emplace(RunnerOptions::shownames);
  }

  showParamTypes = res[RunnerOptions::showtypes].as<bool>();
  if (showParamTypes) {
    m_runnerOptions.emplace(RunnerOptions::showtypes);
  }

  runner = res[RunnerOptions::runner].as<std::string>();

  if (res[RunnerOptions::select].count()) {
    filter.select = res[RunnerOptions::select].as<std::string>();
    foundRunnerOption(RunnerOptions::select);
  }

  if (res[RunnerOptions::exclude].count()) {
    foundRunnerOption(RunnerOptions::exclude);
    filter.exclude = res[RunnerOptions::exclude].as<std::string>();
  }

  //listing runners
}

std::unordered_set<std::string> const& Config::options() const
{
  return m_runnerOptions;
}


void Config::foundRunnerOption(std::string const& option)
{
  m_runnerOptions.emplace(option);
}

} // namespace ftg
