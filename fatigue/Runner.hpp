// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

/** @file */

#ifndef FATIGUE_LOGGER_HPP
#define FATIGUE_LOGGER_HPP

#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

namespace ftg {


class Suite;
class Test;

typedef std::vector<std::variant<std::unique_ptr<Test>, std::unique_ptr<Suite>>> TestList;

/** @brief Interface to define a test Runner, which should take care of running a TestList, recursively. */
struct Runner {
  virtual ~Runner(){};
  virtual std::unordered_set<std::string> supportedOptions() const = 0;
  virtual unsigned run(TestList const& tests) = 0;
};

/** @brief Data structure handling to register name, value and type of a parameter passed to a Check.*/
struct ParamInfo {
  ParamInfo(std::string const& name, std::string const& value, std::string const& type) :
      name(name),
      value(value),
      type(type)
  {
  }
  std::string name;
  std::string value;
  std::string type;

  bool operator==(ParamInfo const& other) const = default;
};


/** @brief Interface to define a test Logger, which should take care of reporting checks in a particular test run. */
struct Logger {
  struct Message {
    enum Mode { MESSAGE_CHECK, MESSAGE_WARN };

    Message(Mode mode,
	    std::string const& description,
	    std::vector<ParamInfo> const& params,
	    bool expected,
	    bool result,
	    bool important);

    Mode mode;
    std::string description;
    std::vector<ParamInfo> params;
    bool expected;
    bool result;
    bool important;

    bool operator==(Message const& other) const = default;
  };

  virtual ~Logger(){};
  virtual void report(Message const& message) = 0;
};

} // namespace ftg

#endif