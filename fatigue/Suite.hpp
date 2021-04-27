#ifndef FATIGUE_SUITE_HPP
#define FATIGUE_SUITE_HPP

#include "Test.hpp"
#include "TestDriver.hpp"
#include "TestRunner.hpp"
#include <variant>

namespace ftg {

class Suite;


class Suite {
public:
  virtual TestList tests() const = 0;

public:
  Suite(std::string const& name);
  virtual ~Suite();

  //is this mandatory ?
  //virtual std::vector<std::unique_ptr<Suite>> uses() const;

  std::string const& name() const;

private:
  std::string m_name;
};

} // namespace ftg

#endif
