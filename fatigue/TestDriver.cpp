#include "TestDriver.hpp"
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

FatalCheckFailure::FatalCheckFailure() {}

CheckReporter::CheckReporter(TestDriver& test,
			     std::string const& description,
			     std::vector<ParamInfo> const& params,
			     bool res) :
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

CheckReporter::~CheckReporter()
{
  report();
}

void CheckReporter::warn()
{
  m_mode = MESSAGE_WARN;
}

void CheckReporter::fatal()
{
  m_mode = MESSAGE_FATAL;
  report();
  if (m_res != m_expected) {
    throw FatalCheckFailure();
  }
}

CheckReporter& CheckReporter::important()
{
  m_important = true;
  return *this;
}

CheckReporter& CheckReporter::fails()
{
  m_expected = false;
  return *this;
}

CheckReporter& CheckReporter::succeeds()
{
  m_expected = true;
  return *this;
}

void CheckReporter::report()
{
  if (m_reported == false) {
    m_reported = true;
    if ((m_res && !m_expected) || (!m_res && m_expected)) {
      m_test.m_logger->checkFailed(m_mode, m_description, m_params, m_expected, m_res, m_important);
    } else {
      m_test.m_logger->checkPassed();
    }
  }
}

TestDriver::TestDriver(std::string const& name) : m_logger(nullptr), m_name(name) {}

void TestDriver::setLogger(TestLogger* r)
{
  m_logger = r;
}
std::string const& TestDriver::name() const
{
  return m_name;
}

} // namespace ftg