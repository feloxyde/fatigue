// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

/** @file */

#ifndef FATIGUE_DEFAULTRUNNER_HPP
#define FATIGUE_DEFAULTRUNNER_HPP

#include "../Config.hpp"
#include "../Runner.hpp"
#include "../Suite.hpp"
#include <cstddef>
#include <cstdint>
#include <exception>
#include <ostream>

namespace ftg {


/** @brief Default test runner.

  Not immune to segfault, but ensures portability. Runs tests and suites sequentially and prints logs to a ```std::ostream```.

*/
class DefaultRunner : public Runner, public Logger {
public:
  DefaultRunner(std::ostream& ostream, Config const& config);
  virtual ~DefaultRunner();
  virtual std::unordered_set<std::string> supportedOptions() const;
  virtual unsigned run(TestList const& tests);

  virtual void report(Logger::Message const& message) noexcept;

protected:
  /** @brief Selects whether to run suite or test for each element of testlist. */
  void dispatchTestList(TestList const& tests, std::vector<std::string> const& prefixes);

  /** @brief Runs dispatchTestList on all elements of the TestList of the suite */
  void runSuite(std::unique_ptr<Suite> const& suite, std::vector<std::string> const& prefixes);

  /** @brief Loads, runs and unloads a test */
  void runTest(std::unique_ptr<Test> const& test, std::vector<std::string> const& prefixes);

  bool runLoadedTest(std::unique_ptr<Test> const& t);

private:
  Config const& m_config;
  std::ostream& m_ostream;
  size_t m_totalPass;
  size_t m_totalFailed;
  size_t m_totalSkipped;
  struct {
    size_t checkPassed;
    size_t checkFailed;
    bool passed;
  } m_currentRun;
};

} // namespace ftg

#endif