#include "OstreamTestRunner.hpp"
#include "fatigue/Config.hpp"
#include "fatigue/Suite.hpp"
#include "fatigue/TestRunner.hpp"
#include <variant>

namespace ftg {

OstreamTestLogger::OstreamTestLogger(std::ostream& ostream) :
    m_ostream(ostream),
    m_passed(true),
    m_checkPassed(0),
    m_checkFailed(0)
{
}

OstreamTestLogger::~OstreamTestLogger()
{
}

void OstreamTestLogger::checkFailed(MessageMode mode,
				    std::string const& description,
				    std::vector<ParamInfo> const& params,
				    bool expected,
				    bool result,
				    bool important)
{
  m_checkFailed++;

  if (important) {
    m_ostream << "!!! ";
  }

  m_ostream << "(" << m_checkFailed + m_checkPassed << ") ";

  if (mode == MESSAGE_CHECK) {
    m_ostream << "[ERROR] ";
    m_passed = false;
  } else if (mode == MESSAGE_FATAL) {
    m_passed = false;
    m_ostream << "[FATAL] ";
  } else if (mode == MESSAGE_WARN) {
    m_ostream << "[WARN] ";
  }
  m_ostream << "expected ";
  m_ostream << description;

  if (params.size() > 0) {
    m_ostream << "( ";
    size_t i = 0;
    for (auto const& p : params) {
      if (ftg::config().showParamNames) {
	m_ostream << p.name << ": ";
      }
      m_ostream << p.value;
      if (ftg::config().showParamTypes) {
	m_ostream << " [" << p.type << "]";
      }
      if (i < params.size() - 1) {
	m_ostream << ", ";
      }
      i++;
    }
    m_ostream << " )";
  }

  if (expected && !result) {
    m_ostream << " to succeed, but failed." << std::endl;
  } else if (!expected && result) {
    m_ostream << " to fail, but succeeded." << std::endl;
  }
}

void OstreamTestLogger::checkPassed()
{
  m_checkPassed++;
}

bool OstreamTestLogger::passed() const
{
  return m_passed;
}

OstreamTestRunner::OstreamTestRunner(std::ostream& ostream) :
    m_ostream(ostream),
    totalPass(0),
    totalFailed(0),
    totalSkipped(0)
{
}

OstreamTestRunner::~OstreamTestRunner()
{
}


unsigned OstreamTestRunner::run(TestList const& tests)
{
  m_ostream << "---------------------------" << std::endl;
  m_ostream << "------ RUNNING TESTS ------" << std::endl;
  m_ostream << "---------------------------";

  //running suites
  std::vector<std::string> prefixes;
  dispatchTestList(tests, prefixes);

  m_ostream << std::endl << std::endl << std::endl << "---------------------------" << std::endl;
  if (totalFailed == 0) {
    m_ostream << "---------- PASSED ---------" << std::endl;
  } else {
    m_ostream << "---------- FAILED ---------" << std::endl;
  }
  m_ostream << "ran : " << totalPass + totalFailed << std::endl;
  if (totalFailed != 0) {
    m_ostream << "failed : " << totalFailed << std::endl;
  }
  if (totalSkipped != 0) {
    m_ostream << "skipped : " << totalSkipped << std::endl;
  }
  m_ostream << "---------------------------" << std::endl;

  return totalFailed;
}


void OstreamTestRunner::dispatchTestList(TestList const& tests, std::vector<std::string> const& prefixes)
{
  static_assert(std::variant_size_v<TestList::value_type> == 2,
		"OstreamTestRunner expects 2 differents alternatives in the TestList variant");
  for (auto& s : tests) {
    if (std::holds_alternative<std::unique_ptr<Test>>(s)) {
      runTest(std::get<std::unique_ptr<Test>>(s), prefixes);
    } else if (std::holds_alternative<std::unique_ptr<Suite>>(s)) {
      runSuite(std::get<std::unique_ptr<Suite>>(s), prefixes);
    }
  }
}

void OstreamTestRunner::runSuite(std::unique_ptr<Suite> const& suite, std::vector<std::string> const& prefixes)
{

  m_ostream << std::endl << std::endl << std::endl;
  m_ostream << "##### " << suite->name() << " #####";

  std::vector<std::string> pre = prefixes;
  pre.push_back(suite->name());
  dispatchTestList(suite->tests(), pre);
}


void OstreamTestRunner::runTest(std::unique_ptr<Test> const& test, std::vector<std::string> const& prefixes)
{
  if (!ftg::config().filter.shouldRun(prefixes, test->name())) {
    m_ostream << std::endl << std::endl << "-- " << test->name() << " -- (skipped)" << std::endl;
    totalSkipped++;
    return;
  }
  m_ostream << std::endl << std::endl << "-- " << test->name() << " --" << std::endl;


  if (test->load()) {
    if (runLoadedTest(test)) {
      totalPass++;
    } else {
      totalFailed++;
    }
  } else {
    m_ostream << "-- failed : error during load phase --";
    totalFailed++;
  }
}


bool OstreamTestRunner::runLoadedTest(std::unique_ptr<Test> const& t)
{
  bool exceptPass = true;
  bool passed = true;

  OstreamTestLogger otl(m_ostream);
  t->setLogger(&otl);

  try {
    t->run();
    t->unload();
  } catch (ftg::FatalCheckFailure& e) {
    m_ostream << "Test ended due to fatal check failing." << std::endl;
    t->unload();
  } catch (...) {
    m_ostream << "[EXCEPTION] uncaught exception detected, test ending." << std::endl;
    exceptPass = false;
    t->unload();
  }

  if (exceptPass && otl.passed()) {
    m_ostream << "-- passed : ";
    passed = true;
  } else {
    m_ostream << "-- failed : ";
    passed = false;
  }

  m_ostream << "out of " << otl.m_checkPassed + otl.m_checkFailed << " checks, " << otl.m_checkFailed << " failed. --";
  return passed;
}

} // namespace ftg
