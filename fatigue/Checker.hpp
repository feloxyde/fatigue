/** */

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

/** @brief Structure used as exception to stop test in case a marked check fails. */
struct EndTestOnFailure final {
  EndTestOnFailure();
};

/** @brief Structure used as exception to stop test in case a marked check succeeds */
struct EndTestOnSuccess final {
  EndTestOnSuccess();
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

  /** @brief Marks check as a warning, allowing test to pass even if check fails. */
  Check& warn();

  /** @brief In case check passes, test is stopped */
  void endRunOnSuccess();

  /** @brief In case check fails, test is stopped */
  void endRunOnFailure();

  /** @brief Marks check as important, hinting runner to emphasis it in case it fails. */
  Check& important();

  /** @brief Marks check as meant to fail, check will fail if condition checked is true */
  Check& fails();

  /** @brief Marks check as meant to succeed, check will fail if condition checked is false. This is default behavior */
  Check& succeeds();

  /**
  @}
  */

private:
  /** @brief reports check result to runner of the test. Used when destroying Check object or when a fatal check fails.*/
  void report();
  Check(Checker& test, std::string const& description, std::vector<ParamInfo> const& params, bool res);

private:
  Checker& m_test;
  std::string m_description;
  bool m_res;
  MessageMode m_mode;
  bool m_expected;
  bool m_important;
  bool m_reported;
  std::vector<ParamInfo> m_params;

private:
  friend Checker;
};

/** 
@} 
*/

/** @brief Class that is base of Test, providing check reporting mechanisms. 

  This class acts in synergy with Check. When creating a Check object, Checker instance is passed to as a reference. 
  When deleted, Check reports back to Checker, messaging its logger.

*/
class Checker {
public:
  Checker(std::string const& name);

  /** @brief sets logger for the test */
  void setLogger(Logger* r);
  /** @brief returns name of the test */
  std::string const& name() const;

  /** @brief Creates a check. 
    
    @param description a description of the check performed
    @param params a descriptions of params passed to the test
    @param res result of the check
  */
  Check raw_check(std::string const& description, std::vector<ParamInfo> const& params, bool res);

protected:
  bool showTypes() const;

private:
  Logger* m_logger;
  std::string m_name;

private:
  friend Check;
};

} // namespace ftg

#endif