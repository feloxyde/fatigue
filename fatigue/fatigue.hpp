
/** @file */


#ifndef FATIGUE_FATIGUE_HPP
#define FATIGUE_FATIGUE_HPP

#include "Config.hpp"
#include "Suite.hpp"
#include "external/cxxopts.hpp"
#include <iterator>

namespace ftg {

/** \addtogroup user_api 
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
class fatigue {

  /** @brief Defines a class of function used to extend ftg::fatigue by declaring more objects. */
  typedef void (*OptionsDeclarer)(cxxopts::Options&);

public:
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

protected:
  /** @brief Constructor available for extension, allowing to set new CLI options through declarer */
  fatigue(std::vector<OptionsDeclarer> const& newOpts, int argc, char** argv);

protected:
  std::unordered_map<std::string, std::unique_ptr<Runner>> m_runners;
  Config m_config;
  cxxopts::ParseResult m_parsedOpts;

private:
  TestList tests;
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