#include "DefaultRunner.hpp"
#include "../Config.hpp"
#include "../Runner.hpp"
#include "../Suite.hpp"
#include "fatigue/Checker.hpp"
#include <variant>

namespace ftg {

DefaultLogger::DefaultLogger(std::ostream& ostream, Config const& config) :
    m_config(config),
    m_ostream(ostream),
    m_passed(true),
    m_checkPassed(0),
    m_checkFailed(0)
{
}

DefaultLogger::~DefaultLogger()
{
}

void DefaultLogger::report(MessageMode mode,
			   std::string const& description,
			   std::vector<ParamInfo> const& params,
			   bool expected,
			   bool result,
			   bool important)
{
  if (expected == result) {
    m_checkPassed++;
    return;
  }

  m_checkFailed++;

  if (important) {
    m_ostream << "!!! ";
  }

  m_ostream << "(" << m_checkFailed + m_checkPassed << ") ";

  if (mode == MESSAGE_CHECK) {
    m_ostream << "[ERROR] ";
    m_passed = false;
  } else if (mode == MESSAGE_WARN) {
    m_ostream << "[WARN] ";
  }

  m_ostream << description;

  if (params.size() > 0) {
    m_ostream << "( ";
    size_t i = 0;
    for (auto const& p : params) {
      if (m_config.showParamNames) {
	m_ostream << p.name << ": ";
      }
      m_ostream << p.value;
      if (m_config.showParamTypes) {
	m_ostream << " [" << p.type << "]";
      }
      if (i < params.size() - 1) {
	m_ostream << ", ";
      }
      i++;
    }
    m_ostream << " )";
  }

  if (expected) {
    m_ostream << " -> true";
  } else {
    m_ostream << " -> false";
  }

  if (expected != result) {
    m_ostream << " : failed." << std::endl;
  }
}

bool DefaultLogger::passed() const
{
  return m_passed;
}

DefaultRunner::DefaultRunner(std::ostream& ostream, Config const& config) :
    m_config(config),
    m_ostream(ostream),
    totalPass(0),
    totalFailed(0),
    totalSkipped(0)
{
}

DefaultRunner::~DefaultRunner()
{
}


std::unordered_set<std::string> DefaultRunner::supportedOptions() const
{
  return {Config::options::shownames, Config::options::showtypes, Config::options::select, Config::options::exclude};
}

unsigned DefaultRunner::run(TestList const& tests)
{
  m_ostream << "------ RUNNING TESTS ------" << std::endl;

  //running suites
  std::vector<std::string> prefixes;
  dispatchTestList(tests, prefixes);

  m_ostream << std::endl;
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

  return totalFailed;
}


void DefaultRunner::dispatchTestList(TestList const& tests, std::vector<std::string> const& prefixes)
{
  static_assert(std::variant_size_v<TestList::value_type> == 2,
		"DefaultRunner expects 2 differents alternatives in the TestList variant");
  for (auto& s : tests) {
    if (std::holds_alternative<std::unique_ptr<Test>>(s)) {
      runTest(std::get<std::unique_ptr<Test>>(s), prefixes);
    } else if (std::holds_alternative<std::unique_ptr<Suite>>(s)) {
      runSuite(std::get<std::unique_ptr<Suite>>(s), prefixes);
    }
  }
}

void DefaultRunner::runSuite(std::unique_ptr<Suite> const& suite, std::vector<std::string> const& prefixes)
{
  std::vector<std::string> pre = prefixes;
  pre.push_back(suite->name());
  dispatchTestList(suite->tests(), pre);
}

static std::string
formatTestName(std::vector<std::string> const& prefixes, std::string const& name, std::string const& separator)
{
  std::string str;
  for (auto const& p : prefixes) {
    str += p + separator;
  }
  str += name;

  return str;
}

void DefaultRunner::runTest(std::unique_ptr<Test> const& test, std::vector<std::string> const& prefixes)
{
  std::string tname = formatTestName(prefixes, test->name(), m_config.filter.separator);

  m_ostream << std::endl << "---- " << tname << std::endl;
  if (!m_config.filter.shouldRun(prefixes, test->name())) {
    m_ostream << "skipped." << std::endl;
    totalSkipped++;
    return;
  }


  if (test->load()) {
    if (runLoadedTest(test)) {
      totalPass++;
    } else {
      totalFailed++;
    }
  } else {
    m_ostream << "---- failed : "
	      << "error during load phase." << std::endl;
    totalFailed++;
  }
}


bool DefaultRunner::runLoadedTest(std::unique_ptr<Test> const& t)
{
  bool exceptPass = true;
  bool passed = true;

  DefaultLogger otl(m_ostream, m_config);
  t->setLogger(&otl);

  try {
    t->run();
    t->unload();
  } catch (ftg::EndRunOnFailure& e) {
    m_ostream << "Test ended on check failure." << std::endl;
    t->unload();
  } catch (ftg::EndRunOnSuccess& e) {
    m_ostream << "Test ended on check success." << std::endl;
    t->unload();
  } catch (...) {
    m_ostream << "[EXCEPTION] uncaught exception detected, test ending." << std::endl;
    exceptPass = false;
    t->unload();
  }

  if (exceptPass && otl.passed()) {
    m_ostream << "---- passed : ";
    passed = true;
  } else {
    m_ostream << "---- failed : ";
    passed = false;
  }

  m_ostream << "out of " << otl.m_checkPassed + otl.m_checkFailed << " checks, " << otl.m_checkFailed << " failed."
	    << std::endl;
  return passed;
}

} // namespace ftg
