#ifndef FATIGUE_FATIGUE_HPP
#define FATIGUE_FATIGUE_HPP

#include "Config.hpp"
#include "Suite.hpp"
#include "fatigue/TestRunner.hpp"
#include <iterator>

namespace ftg {

template<typename T>
concept TestListBuilder = requires(T a)
{
  {
    a()
  }
  ->std::same_as<TestList>;
};

struct FatigueDriver {
  FatigueDriver declare(std::unique_ptr<Suite> suite) const;
  FatigueDriver declare(std::unique_ptr<Test> test) const;
  template<TestListBuilder Builder>
  FatigueDriver declare(Builder b) const;
  unsigned run() const;

private:
  static TestList tests;
};

FatigueDriver fatigue(int argc, char** argv);

template<TestListBuilder Builder>
FatigueDriver FatigueDriver::declare(Builder b) const
{
  auto testlist = b();
  tests.insert(tests.end(), std::make_move_iterator(testlist.begin()), std::make_move_iterator(testlist.end()));
  return FatigueDriver();
}

} // namespace ftg

#endif