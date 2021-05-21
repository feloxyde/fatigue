#include "Checker.hpp"
#include "Runner.hpp"
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

EndRunOnFailure::EndRunOnFailure()
{
}

EndRunOnSuccess::EndRunOnSuccess()
{
}

Check::Check(Checker& test, std::string const& description, std::vector<ParamInfo> const& params, bool res) :
    m_test(test),
    m_description(description),
    m_res(res),
    m_mode(MESSAGE_CHECK),
    m_expected(true),
    m_important(false),
    m_reported(false),
    m_params(params)

{
}

Check::Check(Check&& origin) :
    m_test(origin.m_test),
    m_description(origin.m_description),
    m_res(origin.m_res),
    m_mode(MESSAGE_CHECK),
    m_expected(origin.m_expected),
    m_important(origin.m_important),
    m_reported(origin.m_reported),
    m_params(origin.m_params)
{
  //disabling reporting since object has moved
  origin.m_reported = true;
}

Check::~Check()
{
  report();
}

Check& Check::warn()
{
  m_mode = MESSAGE_WARN;
  return *this;
}

/** @brief In case check passes, test is stopped */
void Check::endRunOnSuccess()
{
  if (m_res == m_expected) {
    report();
    throw EndRunOnSuccess();
  }
}

/** @brief In case check fails, test is stopped */
void Check::endRunOnFailure()
{
  if (m_res != m_expected) {
    report();
    throw EndRunOnFailure();
  }
}

Check& Check::important()
{
  m_important = true;
  return *this;
}

Check& Check::isFalse()
{
  m_expected = false;
  return *this;
}

Check& Check::isTrue()
{
  m_expected = true;
  return *this;
}

void Check::report()
{
  if (m_reported == false) {
    m_reported = true;
    m_test.m_logger->report(m_mode, m_description, m_params, m_expected, m_res, m_important);
  }
}

Checker::Checker(std::string const& name) : m_logger(nullptr), m_name(name)
{
}

void Checker::setLogger(Logger* r)
{
  m_logger = r;
}
std::string const& Checker::name() const
{
  return m_name;
}


Check Checker::raw_check(std::string const& description, std::vector<ParamInfo> const& params, bool res)
{
  return Check(*this, description, params, res);
}

} // namespace ftg