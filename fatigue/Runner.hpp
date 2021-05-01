/** @file */

#ifndef FATIGUE_LOGGER_HPP
#define FATIGUE_LOGGER_HPP

#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace ftg {

enum MessageMode { MESSAGE_CHECK, MESSAGE_WARN, MESSAGE_FATAL, MESSAGE_INFO };

class Suite;
class Test;

typedef std::vector<std::variant<std::unique_ptr<Test>, std::unique_ptr<Suite>>> TestList;

/** @brief Interface to define a test Runner, which should take care of running a TestList, recursively. */
struct Runner {
  virtual ~Runner(){};
  virtual unsigned run(TestList const& suites) = 0;
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
};

/* @brief Interface to define a test Logger, which should take care of reporting checks in a particular test run. */
struct Logger {
  virtual ~Logger(){};
  virtual void checkFailed(MessageMode mode,
			   std::string const& description,
			   std::vector<ParamInfo> const& params,
			   bool expected,
			   bool result,
			   bool important)
      = 0;
  virtual void checkPassed() = 0;
};

} // namespace ftg

#endif