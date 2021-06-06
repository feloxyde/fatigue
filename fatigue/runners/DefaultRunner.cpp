// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "DefaultRunner.hpp"
#include "../Checker.hpp"
#include "../Config.hpp"
#include "../Runner.hpp"
#include "../Suite.hpp"
#include <variant>

namespace ftg {


DefaultRunner::DefaultRunner(std::ostream& ostream, Config const& config) :
    m_config(config),
    m_ostream(ostream),
    m_totalPass(0),
    m_totalFailed(0),
    m_totalSkipped(0)
{
}

DefaultRunner::~DefaultRunner()
{
}


std::unordered_set<std::string> DefaultRunner::supportedOptions() const
{
  return {Config::RunnerOptions::shownames,
	  Config::RunnerOptions::showtypes,
	  Config::RunnerOptions::select,
	  Config::RunnerOptions::exclude};
}

unsigned DefaultRunner::run(TestList const& tests)
{
  m_ostream << "------ RUNNING TESTS ------" << std::endl;

  //running suites
  std::vector<std::string> prefixes;
  dispatchTestList(tests, prefixes);

  m_ostream << std::endl;
  if (m_totalFailed == 0) {
    m_ostream << "---------- PASSED ---------" << std::endl;
  } else {
    m_ostream << "---------- FAILED ---------" << std::endl;
  }
  m_ostream << "ran : " << m_totalPass + m_totalFailed << std::endl;
  if (m_totalFailed != 0) {
    m_ostream << "failed : " << m_totalFailed << std::endl;
  }
  if (m_totalSkipped != 0) {
    m_ostream << "skipped : " << m_totalSkipped << std::endl;
  }

  return m_totalFailed;
}


void DefaultRunner::report(Logger::Message const& message)
{
  if (message.expected == message.result) {
    m_currentRun.checkPassed++;
    return;
  }

  m_currentRun.checkFailed++;

  if (message.important) {
    m_ostream << "!!! ";
  }

  m_ostream << "(" << m_currentRun.checkFailed + m_currentRun.checkPassed << ") ";

  if (message.mode == Logger::Message::MESSAGE_CHECK) {
    m_ostream << "[ERROR] ";
    m_currentRun.passed = false;
  } else if (message.mode == Logger::Message::MESSAGE_WARN) {
    m_ostream << "[WARN] ";
  }

  m_ostream << message.description;

  if (message.params.size() > 0) {
    m_ostream << "( ";
    size_t i = 0;
    for (auto const& p : message.params) {
      if (m_config.showParamNames) {
	m_ostream << p.name << ": ";
      }
      m_ostream << p.value;
      if (m_config.showParamTypes) {
	m_ostream << " [" << p.type << "]";
      }
      if (i < message.params.size() - 1) {
	m_ostream << ", ";
      }
      i++;
    }
    m_ostream << " )";
  }

  if (message.expected) {
    m_ostream << " -> true";
  } else {
    m_ostream << " -> false";
  }

  if (message.expected != message.result) {
    m_ostream << " : failed." << std::endl;
  }
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
    m_totalSkipped++;
    return;
  }

  m_currentRun.checkPassed = 0;
  m_currentRun.checkFailed = 0;
  m_currentRun.passed = true;
  bool loadFailure = false;
  // clang-format off
  Checker::run(*test, *this, 
          //case of clean exit
          [](){},
          //case of load failure
	        [this, &loadFailure](){
            this->m_totalFailed++;
            this->m_currentRun.passed = false;
            loadFailure = true;
          },
          //case of check failure direct exit 
          [this](){  
            this->m_ostream << "Test ended on check " << m_currentRun.checkFailed + m_currentRun.checkPassed << " failure." << std::endl;
          },
          //case of check success direct exit
	        [this](){
            this->m_ostream << "Test ended on check " << m_currentRun.checkFailed + m_currentRun.checkPassed << " success." << std::endl;
          },
	        //case of uncaught exception is thrown 
          [this](){
            this->m_ostream << "[EXCEPTION] unexpected exception thrown after check " << m_currentRun.checkFailed + m_currentRun.checkPassed << ", test ending." << std::endl;
            this->m_currentRun.passed = false;
          }
  );
  // clang-format on

  if (loadFailure) {
    m_ostream << "---- failed : unable to load." << std::endl;
    return;
  }

  if (m_currentRun.passed) {
    m_ostream << "---- passed : ";
    m_totalPass++;
  } else {
    m_ostream << "---- failed : ";
    m_totalFailed++;
  }

  m_ostream << m_currentRun.checkPassed + m_currentRun.checkFailed << " checks, " << m_currentRun.checkFailed
	    << " failed." << std::endl;
}
} // namespace ftg
