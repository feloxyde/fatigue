
/** @file */


#ifndef FATIGUE_FATIGUE_HPP
#define FATIGUE_FATIGUE_HPP

#include "Config.hpp"
#include "Suite.hpp"
#include <iterator>

namespace ftg {

/** \addtogroup UserApi 
@{ 
*/

template<typename T>
concept TestListBuilder = requires(T a)
{
  {
    a()
  }
  ->std::same_as<TestList>;
};

/** @brief fatigue structure, which is the top level class, registering tests and coordinating others. */
struct fatigue {

  /** @brief Constructor, meant to be called with ```main``` function arguments */
  fatigue(int argc, char** argv);

  /** @brief declares a suite */
  fatigue& declare(std::unique_ptr<Suite> suite);

  /** @brief declares a test */
  fatigue& declare(std::unique_ptr<Test> test);

  /** @brief Uses an invokable to create a TestList and appends it to tests. */
  template<TestListBuilder Builder>
  fatigue& declare(Builder b);

  /** @brief runs tests with current config */
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

/**
@}
*/

} // namespace ftg


#endif