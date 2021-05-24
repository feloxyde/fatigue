// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

/** @file */

#ifndef FATIGUE_CHECKER_HPP
#define FATIGUE_CHECKER_HPP
#include "Runner.hpp"
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

/** @brief Structure used as exception to stop test in case a marked check
 * fails. */
struct EndRunOnFailure final {
  EndRunOnFailure();
};

/** @brief Structure used as exception to stop test in case a marked check
 * succeeds */
struct EndRunOnSuccess final {
  EndRunOnSuccess();
};

class Checker;

/** \addtogroup user_api
@{
*/
/** @brief Allows customized behavior when checking in Test class.

  see Checker class documentation for more info.
*/
class Check final {
public:
  Check(Check const&) = delete;
  Check(Check&& origin);

  ~Check();

  /**
  \addtogroup checks
  @{
  */

  /** @brief Marks check as a warning, allowing test to pass even if check
   * fails. */
  Check& warn();

  /** @brief In case check passes, test is stopped */
  void endRunOnSuccess();

  /** @brief In case check fails, test is stopped */
  void endRunOnFailure();

  /** @brief Marks check as important, hinting runner to emphasis it in case it
   * fails. */
  Check& important();

  /** @brief Marks check as meant to fail, check will fail if condition checked
   * is true */
  Check& isFalse();

  /** @brief Marks check as meant to succeed, check will fail if condition
   * checked is false. This is default behavior */
  Check& isTrue();

  /**
  @}
  */

private:
  /** @brief reports check result to runner of the test. Used when destroying
   * Check object or when a fatal check fails.*/
  void report();
  Check(Checker& test, std::string const& description, std::vector<ParamInfo> const& params, bool res);

private:
  Checker& m_test;
  bool m_reported;
  std::string m_description;
  bool m_res;
  Logger::Message::Mode m_mode;
  bool m_expected;
  bool m_important;
  std::vector<ParamInfo> m_params;

private:
  friend Checker;
};

/**
@}
*/

template<typename T>
concept NoParamCallable = requires(T const& a)
{
  {
    a()
  }
  ->std::same_as<void>;
};

/** @brief Class that is base of Test, providing check reporting mechanisms.

  This class acts in synergy with Check. When creating a Check object,
  Checker instance is passed to as a reference. When deleted, Check reports
  back to Checker, messaging its logger.

*/
class Checker {
public:
  /** @brief Runs a test, ensuring all possible exit cases are handled, and calls are made in right order. */
  template<NoParamCallable OnCleanExitLambda,
	   NoParamCallable OnLoadFailureLambda,
	   NoParamCallable OnCheckFailureExitLambda,
	   NoParamCallable OnCheckSuccessExitLambda,
	   NoParamCallable OnExceptionExitLabmda>
  static void run(Checker& checker,
		  Logger& logger,
		  OnCleanExitLambda const& onCleanExit,
		  OnLoadFailureLambda const& onLoadFailure,
		  OnCheckFailureExitLambda const& onCheckFailureExit,
		  OnCheckSuccessExitLambda const& onCheckSuccessExit,
		  OnExceptionExitLabmda const& onExceptionExit);

public:
  Checker(std::string const& name);

  /** @brief returns name of the test */
  std::string const& name() const;

protected:
  /** @brief Method meant to be overriden, which is the test execution scenario.
   */
  virtual void run() = 0;

  /** @brief Method meant to be optionally overriden, which is run before
   * execution of scenario. */
  virtual bool load() noexcept { return true; }

  /** @brief Method meant to be optionally overriden, which is run after
   * execution of scenario. */
  virtual void unload() noexcept {}


  /** @brief Creates a check.

    @param description a description of the check performed
    @param params a descriptions of params passed to the test
    @param res result of the check
  */
  Check raw_check(std::string const& description, std::vector<ParamInfo> const& params, bool res);

private:
  /** @brief sets logger for the test */
  void setLogger(Logger* r);

private:
  Logger* m_logger;
  std::string m_name;


private:
  friend Check;
};


template<NoParamCallable OnCleanExitLambda,
	 NoParamCallable OnLoadFailureLambda,
	 NoParamCallable OnCheckFailureExitLambda,
	 NoParamCallable OnCheckSuccessExitLambda,
	 NoParamCallable OnExceptionExitLabmda>
void Checker::run(Checker& checker,
		  Logger& logger,
		  OnCleanExitLambda const& onCleanExit,
		  OnLoadFailureLambda const& onLoadFailure,
		  OnCheckFailureExitLambda const& onCheckFailureExit,
		  OnCheckSuccessExitLambda const& onCheckSuccessExit,
		  OnExceptionExitLabmda const& onExceptionExit)
{
  checker.setLogger(&logger);

  if (checker.load()) {
    try {
      checker.run();
      onCleanExit();
      checker.unload();
    } catch (ftg::EndRunOnFailure& e) {
      onCheckFailureExit();
      checker.unload();
    } catch (ftg::EndRunOnSuccess& e) {
      onCheckSuccessExit();
      checker.unload();
    } catch (...) {
      onExceptionExit();
      checker.unload();
    }
  } else {
    onLoadFailure();
  }
}


} // namespace ftg

#endif