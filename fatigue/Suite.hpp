/**

*/

/** @file */

/** \addtogroup UserApi 
@{ 
*/

#ifndef FATIGUE_SUITE_HPP
#define FATIGUE_SUITE_HPP

#include "Test.hpp"
#include <variant>

namespace ftg {

class Suite;

/** @brief Suite class, meant to group test. */
class Suite {
public:
  /** @brief Method meant to be overriden to define tests in suite */
  virtual TestList tests() const = 0;

public:
  /** @brief Suite constructor */
  Suite(std::string const& name);

  virtual ~Suite();

  //is this mandatory ?
  //virtual std::vector<std::unique_ptr<Suite>> uses() const;

  /** @brief Returns name of the suite */
  std::string const& name() const;

private:
  std::string m_name;
};


} // namespace ftg


/**
@}
*/

#endif
