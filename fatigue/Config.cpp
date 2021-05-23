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

  showParamNames = res[options::shownames].as<bool>();
  if (showParamNames) {
    m_options.emplace(options::shownames);
  }

  showParamTypes = res[options::showtypes].as<bool>();
  if (showParamTypes) {
    m_options.emplace(options::showtypes);
  }

  runner = res[options::runner].as<std::string>();

  if (res[options::select].count()) {
    filter.select = res[options::select].as<std::string>();
    m_options.emplace(options::select);
  }

  if (res[options::exclude].count()) {
    m_options.emplace(options::exclude);
    filter.exclude = res[options::exclude].as<std::string>();
  }

  //listing runners
}

std::unordered_set<std::string> const& Config::options() const
{
  return m_options;
}


void Config::setOption(std::string const& option)
{
  m_options.emplace(option);
}

} // namespace ftg
