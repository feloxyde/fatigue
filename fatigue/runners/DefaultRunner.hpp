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

/** @brief Default test logger.

  Not immune to segfault, but ensures portability. Prints logs sequentially to a ```std::ostream```.

*/
struct DefaultLogger : public Logger {

  DefaultLogger(std::ostream& ostream, Config const& config);
  virtual ~DefaultLogger();

  virtual void report(MessageMode mode,
		      std::string const& description,
		      std::vector<ParamInfo> const& params,
		      bool expected,
		      bool result,
		      bool important);

  bool passed() const;

public:
  Config const& m_config;
  std::ostream& m_ostream;
  bool m_passed;
  size_t m_checkPassed;
  size_t m_checkFailed;
};


/** @brief Default test runner.

  Not immune to segfault, but ensures portability. Runs tests and suites sequentially and prints logs to a ```std::ostream```.

*/
struct DefaultRunner : public Runner {
  DefaultRunner(std::ostream& ostream, Config const& config);
  virtual ~DefaultRunner();
  virtual unsigned run(TestList const& tests);


private:
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
  size_t totalPass;
  size_t totalFailed;
  size_t totalSkipped;
};

} // namespace ftg

#endif