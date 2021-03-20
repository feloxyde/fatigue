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
  FatalCheckFailure();
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
  CheckReporter(TestDriver& test, std::string const& description, std::vector<ParamInfo> const& params, bool res);

  ~CheckReporter();

  void warn();

  void fatal();

  CheckReporter& important();

  CheckReporter& fails();

  CheckReporter& succeeds();

private:
  void report();
};

class TestDriver
{
public:
  TestDriver(std::string const& name);

public:
  void setLogger(TestLogger* r);
  void setShowTypes(bool show);
  std::string const& name() const;

protected:
  bool showTypes() const;

private:
  bool m_showTypes;
  TestLogger* m_logger;
  std::string m_name;

private:
  friend CheckReporter;
};


} // namespace ftg

#endif