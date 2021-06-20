// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

/** @file */


#ifndef FATIGUE_FATIGUE_HPP
#define FATIGUE_FATIGUE_HPP

#include "Config.hpp"
#include "Suite.hpp"
#include "external/cxxopts.hpp"
#include <iterator>
#include <ostream>

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
  fatigue(int argc, char** argv, std::ostream& out = std::cout);

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
  fatigue(std::vector<OptionsDeclarer> const& newOpts, int argc, char** argv, std::ostream& out = std::cout);

protected:
  std::unordered_map<std::string, std::unique_ptr<Runner>> m_runners;
  Config m_config;
  cxxopts::ParseResult m_parsedOpts;
  /** @brief outstream used for output, for runners supporting custom output */
  std::ostream& m_out;

private:
  TestList m_tests;

public:
  struct ExitCodes {
    static int constexpr unsupportedOption = -2;
    static int constexpr runnerNotFound = -1;
    static int constexpr success = 0;
  };
};

/**
@}
*/

/* IMPLEMENTATION */

template<TestListBuilder Builder>
fatigue& fatigue::declare(Builder b)
{
  auto testlist = b();
  m_tests.insert(m_tests.end(), std::make_move_iterator(testlist.begin()), std::make_move_iterator(testlist.end()));
  return *this;
}


} // namespace ftg


#endif