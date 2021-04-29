#ifndef FATIGUE_FATIGUE_HPP
#define FATIGUE_FATIGUE_HPP

#include "Config.hpp"
#include "Suite.hpp"
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

struct fatigue {

  fatigue(int argc, char** argv);

  fatigue& declare(std::unique_ptr<Suite> suite);
  fatigue& declare(std::unique_ptr<Test> test);
  template<TestListBuilder Builder>
  fatigue& declare(Builder b);
  unsigned run() const;

private:
  TestList tests;
  Config config;
};


template<TestListBuilder Builder>
fatigue& fatigue::declare(Builder b)
{
  auto testlist = b();
  tests.insert(tests.end(), std::make_move_iterator(testlist.begin()), std::make_move_iterator(testlist.end()));
  return *this;
}

} // namespace ftg

#endif