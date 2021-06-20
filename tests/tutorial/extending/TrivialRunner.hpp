// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#ifndef FATIGUE_TUTORIAL_TRIVIALRUNNER_HPP
#define FATIGUE_TUTORIAL_TRIVIALRUNNER_HPP

#include "fatigue/Config.hpp"
#include "fatigue/Runner.hpp"
#include <fatigue/fatigue.hpp>
#include <string>
#include <unordered_set>

/* The overall idea of this trivial runner is : 
  - run tests. 
  - if whatever happens, throws a TrivialRunnerFail exception
*/
struct TrivialRunnerFail {
}; //used for exception

class TrivialRunner : public ftg::Logger, public ftg::Runner {
private:
  bool m_werror;
  bool m_failure;

public:
  /* creating a constructor, and passing it required configuration : the werror option*/
  TrivialRunner(bool werror) : m_werror(werror), m_failure(false) {}
  virtual ~TrivialRunner() {}


  /* Method from Runner. 
    Implement in order to tell which runner-targeted options are supported by the runner
    Empty in our case, since we don't support any runner-targed option
  */
  virtual std::unordered_set<std::string> supportedOptions() const
  {
    std::unordered_set<std::string> opts;
    opts.emplace("werror");
    return opts;
  }

  /* Method from Logger. 
    Implement in order to receive messages from Checker when running tests
    note that arg names are commented out to prevent compiler unused variable warning,
    but for a more elaborated runner, you may want to use them.
    The method is noexcept because it is triggered withing destructor of Check class.
  */
  virtual void report(Logger::Message const& message) noexcept
  {
    //if the message is not a warning, or if we fail on warning
    if (message.mode != ftg::Logger::Message::MESSAGE_WARN || m_werror) {
      //if result isnt what we expect
      if (message.result != message.expected) {
	m_failure = true;
      }
    }
  }

  /* helper method. Rules loading, running and unloading test */
  void runTest(ftg::Test& test)
  {
    //this method handles a test run, and ensures all exit cases are handled
    // clang-format off
    ftg::Checker::run(test, *this,
      /* on a clean exit */
	    [](){},
      /* on load failure */
	    [](){throw TrivialRunnerFail();},
      /* on check failure exit */
	    [](){},
      /* on check success exit */
	    [](){},
      /* on unhandled exception exit */
	    [](){throw TrivialRunnerFail();}
    );
    // cang-format on
    if(m_failure){
      throw TrivialRunnerFail();
    }
  }

  /* helper method, used to run individual tests from TestList */
  void runTests(ftg::TestList const& tests)
  {
    for (auto& test : tests) {
      if (std::holds_alternative<std::unique_ptr<ftg::Suite>>(test)) {
	this->runTests(std::get<std::unique_ptr<ftg::Suite>>(test)->tests());
      } else {
	runTest(*(std::get<std::unique_ptr<ftg::Test>>(test)));
      }
    }
  }

  /* Method from Runner. 
    Implement to run all tests and return 0 if passed, a nonzero positive value if failed.
  */
  virtual unsigned run(ftg::TestList const& tests)
  {
    try {
      runTests(tests);
    } catch (...) {
      std::cout << "FAIL" << std::endl;
      return 1;
    }
    std::cout << "PASS" << std::endl;
    return 0;
  }
};

#endif