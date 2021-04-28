#ifndef FATIGUE_CHECKER_HPP
#define FATIGUE_CHECKER_HPP
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

struct FatalCheckFailure final {
  FatalCheckFailure();
};

class Checker;

class Check final {
private:
  Checker& m_test;
  std::string m_description;
  bool m_res;
  MessageMode m_mode;
  bool m_expected;
  bool m_important;
  bool m_reported;
  std::vector<ParamInfo> m_params;

public:
  Check(Checker& test, std::string const& description, std::vector<ParamInfo> const& params, bool res);

  ~Check();

  void warn();

  void fatal();

  Check& important();

  Check& fails();

  Check& succeeds();

private:
  void report();
};

class Checker {
public:
  Checker(std::string const& name);

public:
  void setLogger(TestLogger* r);
  std::string const& name() const;

protected:
  bool showTypes() const;

private:
  TestLogger* m_logger;
  std::string m_name;

private:
  friend Check;
};

} // namespace ftg

#endif