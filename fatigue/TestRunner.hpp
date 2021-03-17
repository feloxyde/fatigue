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
  virtual void configure(int argc, const char** argv) = 0;
  virtual void run(std::vector<std::unique_ptr<Suite>> const& suites) = 0;
};

struct TestLogger {
  virtual ~TestLogger(){};
  virtual void checkFailed(MessageMode mode, std::string const& description, bool important) = 0;
  virtual void checkPassed() = 0;
};

}


#endif