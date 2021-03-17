#ifndef FATIGUE_OSTREAMTESTRUNNER_HPP
#define FATIGUE_OSTREAMTESTRUNNER_HPP

#include "TestRunner.hpp"
#include "Suite.hpp"  
#include <cstddef>
#include <ostream>


namespace ftg {

struct OstreamTestLogger : public TestLogger {

  OstreamTestLogger(std::ostream& ostream) : m_ostream(ostream){}
  virtual ~OstreamTestLogger(){}
  virtual void checkFailed(MessageMode mode, std::string const& description, bool important) = 0;
  virtual void checkPassed() = 0;

private: 
  std::ostream& m_ostream;
};

struct OstreamTestRunner : public TestRunner
{
  OstreamTestRunner(std::ostream& ostream) : m_ostream(ostream){}
  virtual ~OstreamTestRunner(){}
  virtual void configure(int argc, const char** argv){/* nothing to do here */}
  virtual void run(std::vector<std::unique_ptr<Suite>> const& suites);

private:
  std::ostream& m_ostream;
};

}

#endif