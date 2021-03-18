#ifndef FATIGUE_TESTDRIVER_HPP
#define FATIGUE_TESTDRIVER_HPP
#include "TestRunner.hpp"
#include "utils.hpp"
#include <concepts>
#include <cstddef>
#include <exception>
#include <functional>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace ftg {

//#FIXME add a decoupled "output" (ReportDisplay ?) class for message to allow
// user to
// customize its output !


//#FIXME count checks when they pass/fail, and report number in case of uncaught
// exception.

struct FatalCheckFailure final
{
  FatalCheckFailure(){};
};

class TestDriver;

class CheckReporter final
{
private:
  std::string m_description;
  MessageMode m_mode;
  bool m_res;
  bool m_expected;
  bool m_important;
  TestDriver& m_test;
  bool m_reported;
  std::vector<ParamInfo> m_params;

public:
  CheckReporter(TestDriver& test, std::string const& description, std::vector<ParamInfo> const& params, bool res)
    : m_test(test)
    , m_description(description)
    , m_res(res)
    , m_mode(MESSAGE_CHECK)
    , m_expected(true)
    , m_important(false)
    , m_reported(false)
    , m_params(params)
    
  {}

  ~CheckReporter() { report(); }

  void warn() { m_mode = MESSAGE_WARN; }

  void fatal()
  {
    m_mode = MESSAGE_FATAL;
    report();
    if (m_res != m_expected) {
      throw FatalCheckFailure();
    }
  }

  CheckReporter& important()
  {
    m_important = true;
    return *this;
  }

  CheckReporter& fails()
  {
    m_expected = false;
    return *this;
  }

  CheckReporter& succeeds()
  {
    m_expected = true;
    return *this;
  }

private:
  void report();
};

class TestDriver
{
public:
  TestDriver(std::string const& name)
    : m_showTypes(false)
    , m_logger(nullptr)
    , m_name(name)
  {}

public:
  void setLogger(TestLogger* r) { m_logger = r; }
  void setShowTypes(bool show) { m_showTypes = show; }
  std::string const& name() const { return m_name; }

protected:
  bool showTypes() const { return m_showTypes; }

private:
  bool m_showTypes;
  TestLogger* m_logger;
  std::string m_name;

private:
  friend CheckReporter;
};

/* ### IMPLEMENTATION ### */

inline void
CheckReporter::report()
{
  if (m_reported == false) {
    m_reported = true;
    if ((m_res && !m_expected) || (!m_res && m_expected)) {
      std::cout << "reporting because a failure occurred" << std::endl;
      std::string msg = "expected ";
      msg += m_description;
      if (m_expected == true) {
        msg += " to succeed, but failed.";
      } else {
        msg += " to fail, but succeeded.";
      }
      m_test.m_logger->checkFailed(m_mode, msg, m_params, m_important);
    } else {
      m_test.m_logger->checkPassed();
    }
  }
}

} // namespace ftg

#endif