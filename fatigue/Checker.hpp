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

/** @brief Structure used as exception to stop test run on fatal check failing. */
struct FatalCheckFailure final {
  FatalCheckFailure();
};

class Checker;

/** \addtogroup UserApi
@{
*/
/** @brief Allows customized behavior when checking in Test class. 

  see Checker class documentation for more info.
*/
class Check final {
public:
  /** @brief Creates a check. 
    
    @param test the checker
    @param description a description of the check performed
    @param params a descriptions of params passed to the test
    @param res result of the check
  */
  Check(Checker& test, std::string const& description, std::vector<ParamInfo> const& params, bool res);
  Check(Check const&) = delete;
  Check(Check&& origin);

  ~Check();

  /**
  \addtogroup checks 
  @{
  */

  /** @brief Marks check as a warning, allowing test to pass even if check fails. */
  void warn();

  /** @brief Marks check as fatal, terminating test execution in case it fails. */
  void fatal();

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

private:
  Checker& m_test;
  std::string m_description;
  bool m_res;
  MessageMode m_mode;
  bool m_expected;
  bool m_important;
  bool m_reported;
  std::vector<ParamInfo> m_params;
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