#ifndef FATIGUE_TUTORIAL_TRIVIALRUNNER_HPP
#define FATIGUE_TUTORIAL_TRIVIALRUNNER_HPP

#include "fatigue/Runner.hpp"
#include <fatigue/fatigue.hpp>

/* The overall idea of this trivial runner is : 
  - run tests. 
  - if whatever happens, throws a TrivialRunnerFail exception
*/
struct TrivialRunnerFail {
}; //used for exception

class TrivialRunner : public ftg::Logger, public ftg::Runner {
public:
  /* Method from Logger. 
    Implement in order to receive messages from Checker when running tests
  */
  virtual void report(ftg::MessageMode mode,
		      std::string const& description,
		      std::vector<ftg::ParamInfo> const& params,
		      bool expected,
		      bool result,
		      bool important)
  {
    if (result != expected) {
      /* if result is not what we expect, fail */
      throw TrivialRunnerFail();
    }
  }

  /* helper method. Rules loading, running and unloading test */
  void runTest(ftg::Test& test)
  {
    if (!test.load()) {
      /* if load failed */
      throw TrivialRunnerFail();
    }

    try {
      test.run();
    } catch (...) {
      /* whatever exception, we throw to stop and return error */
      test.unload();
      throw TrivialRunnerFail();
    }
    test.unload();
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
    } catch (TrivialRunnerFail) {
      return 1;
    }

    return 0;
  }
};

#endif