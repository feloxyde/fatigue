#ifndef FATIGUE_OSTREAMTESTRUNNER_HPP
#define FATIGUE_OSTREAMTESTRUNNER_HPP

#include "Config.hpp"
#include "Suite.hpp"
#include "TestRunner.hpp"
#include "fatigue/TestDriver.hpp"
#include <cstddef>
#include <cstdint>
#include <exception>
#include <ostream>

namespace ftg {

struct OstreamTestLogger : public TestLogger {

  OstreamTestLogger(std::ostream& ostream);
  virtual ~OstreamTestLogger();

  virtual void checkFailed(MessageMode mode,
			   std::string const& description,
			   std::vector<ParamInfo> const& params,
			   bool expected,
			   bool result,
			   bool important);

  virtual void checkPassed();

  bool passed() const;

public:
  std::ostream& m_ostream;
  bool m_passed;
  size_t m_checkPassed;
  size_t m_checkFailed;
};

struct OstreamTestRunner : public TestRunner {
  OstreamTestRunner(std::ostream& ostream);
  virtual ~OstreamTestRunner();
  virtual unsigned run(TestList const& tests);


private:
  bool runLoadedTest(std::unique_ptr<Test> const& t);
  void runSuite(std::unique_ptr<Suite> const& suite, std::vector<std::string> const& prefixes);


  struct SuiteVisitor {
    SuiteVisitor(OstreamTestRunner& runner, std::vector<std::string> const& prefixes);

    void operator()(std::unique_ptr<Test> const& t);
    void operator()(std::unique_ptr<Suite> const& t);

    OstreamTestRunner& runner;
    std::vector<std::string> prefixes;
  };

  friend SuiteVisitor;

private:
  std::ostream& m_ostream;
  size_t totalPass;
  size_t totalFailed;
  size_t totalSkipped;
};

} // namespace ftg

#endif