#ifndef FATIGUE_SUITE_HPP
#define FATIGUE_SUITE_HPP

#include "Test.hpp"
#include "TestDriver.hpp"

namespace ftg {


typedef std::vector<std::unique_ptr<Test>> TestList;

class Suite
{
public:
  Suite(std::string const& name);
  virtual ~Suite();

  //is this mandatory ?
  virtual std::vector<std::unique_ptr<Suite>> uses() const;

  virtual TestList tests() const = 0;

  std::string const& name() const;

private:
  std::string m_name;
};


}

#endif
