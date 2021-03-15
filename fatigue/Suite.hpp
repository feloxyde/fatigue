#ifndef FATIGUE_SUITE_HPP
#define FATIGUE_SUITE_HPP

#include "Test.hpp"
#include "TestDriver.hpp"

namespace ftg {

struct TestReport
{
  std::string testName;
  std::vector<LogMessage> testLog;
  std::string message;
  bool passed;
};

struct SuiteReport
{
  std::string suiteName;
  std::vector<TestReport> reports;
  std::string message;
  size_t testFailed;
  size_t testRan;
};

typedef std::vector<std::unique_ptr<Test>> TestList;

class Suite
{
  Suite(std::string const& name);

  SuiteReport run() const; // Will need to add ignore pattern latter on.

  virtual std::unique_ptr<Suite> uses() const {};

  virtual TestList listTest() const = 0;

  std::string const& name() const;

private:
  std::string m_name;
};

inline Suite::Suite(std::string const& name)
  : m_name()
{}

inline SuiteReport
Suite::run()
{
  SuiteReport sr;
  sr.suiteName = m_name;
  sr.testFailed = 0;
  sr.testRan = 0;
  for (auto& test : this->listTest()) {
    sr.testRan++;
    TestReport tr;
    tr.testName = test->name();
    tr.passed = false;

    if (test->load()) {
      try {
        test->run();
        if (test->passed()) {
          tr.passed = true;
          tr.message = "passed";
        }
      } catch (...) {
        tr.message = "Aborted because of uncaught exception";
      }
      tr.log = test->log();
    } else {
      sr
    }
  }
}

inline std::string const&
Suite::name() const
{
  return m_name;
}

}

#endif
