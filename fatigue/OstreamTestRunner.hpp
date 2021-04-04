#ifndef FATIGUE_OSTREAMTESTRUNNER_HPP
#define FATIGUE_OSTREAMTESTRUNNER_HPP

#include "TestRunner.hpp"
#include "Suite.hpp"  
#include "fatigue/TestDriver.hpp"
#include <cstddef>
#include <exception>
#include <ostream>
#include "Config.hpp"


namespace ftg {

struct OstreamTestLogger : public TestLogger {

  OstreamTestLogger(std::ostream& ostream);
  virtual ~OstreamTestLogger();

  virtual void checkFailed(MessageMode mode, std::string const& description, std::vector<ParamInfo> const& params, bool expected, bool result, bool important);

  virtual void checkPassed();

  bool passed() const;

public: 
  std::string m_suite;
  std::ostream& m_ostream;
  size_t m_checkPassed;
  size_t m_checkFailed;
  bool m_passed;
};

struct OstreamTestRunner : public TestRunner
{
  OstreamTestRunner(std::ostream& ostream);
  virtual ~OstreamTestRunner();
  virtual unsigned run(std::vector<std::unique_ptr<Suite>> const& suites);

private:
  bool runLoadedTest(std::unique_ptr<Test> & t, OstreamTestLogger& otl);

private:
  std::ostream& m_ostream;
};

}

#endif