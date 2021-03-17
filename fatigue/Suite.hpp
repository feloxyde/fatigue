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

  //is this mandatory ?
  virtual std::vector<std::unique_ptr<Suite>> uses() const 
  {
    return std::move(std::vector<std::unique_ptr<Suite>>());
  }

  virtual TestList tests() const = 0;

  std::string const& name() const;

private:
  std::string m_name;
};

inline Suite::Suite(std::string const& name)
  : m_name()
{}

inline std::string const&
Suite::name() const
{
  return m_name;
}

}

#endif
