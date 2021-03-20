#ifndef FATIGUE_LOGGER_HPP
#define FATIGUE_LOGGER_HPP

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <iostream>

namespace ftg {

enum MessageMode
{
  MESSAGE_CHECK,
  MESSAGE_WARN,
  MESSAGE_FATAL,
  MESSAGE_INFO
};

class Suite;

struct TestRunner 
{
  virtual ~TestRunner(){};
  virtual void run(std::vector<std::unique_ptr<Suite>> const& suites) = 0;
};

struct ParamInfo {
  ParamInfo(std::string const& name, std::string const& value, std::string const& type) : name(name), value(value), type(type){}
  std::string name;
  std::string value;
  std::string type;
};

struct TestLogger {
  virtual ~TestLogger(){};
  virtual void checkFailed(MessageMode mode, std::string const& description, std::vector<ParamInfo> const& params, bool expected, bool result, bool important) = 0;
  virtual void checkPassed() = 0;
};

}


#endif