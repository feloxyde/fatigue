#ifndef FATIGUE_FATIGUE_HPP
#define FATIGUE_FATIGUE_HPP

#include "Config.hpp"
#include "Suite.hpp"
#include <iterator>

namespace ftg {

typedef std::vector<std::unique_ptr<Suite>> SuiteList;

template<typename T>
concept SuiteBuilder = requires(T a)
{
  {
    a()
  }
  ->std::same_as<SuiteList>;
};

struct FatigueDriver {
  FatigueDriver declare(std::unique_ptr<Suite> suite) const;
  template<SuiteBuilder Builder>
  FatigueDriver declare(Builder b) const;
  unsigned run() const;

private:
  static SuiteList suites;
};

FatigueDriver fatigue(int argc, char** argv);

template<SuiteBuilder Builder>
FatigueDriver FatigueDriver::declare(Builder b) const
{
  auto suitelist = b();
  suites.insert(suites.end(), std::make_move_iterator(suitelist.begin()), std::make_move_iterator(suitelist.end()));
  return FatigueDriver();
}

} // namespace ftg

#endif