// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include <fatigue/Test.hpp>

#include "../../utils/MockRunner.hpp"

#include <cassert>
#include <sstream>

using namespace ftg;


struct TestAutoMsgPass : public Test {
  TestAutoMsgPass() : Test("TestAutoMsgPass") {}

protected:
  virtual void run()
  {
    check_equal(10, 10);

    check_near_equal(10, 12, 2);
    check_near_equal(10, 10, 2);
    check_near_equal(12, 10, 2);

    check_not_equal(10, 12);

    check_less_than(8, 10);

    check_less_equal(8, 10);
    check_less_equal(10, 10);

    check_greater_than(10, 9);

    check_greater_equal(10, 8);
    check_greater_equal(10, 10);

    check_throw<int>([]() { throw(int) 1; });

    check_nothrow([]() {});

    check_true(true);

    check_false(false);
  }
};

class TestRunner : public DefaultRunner {
  TestRunner();
  virtual ~TestRunner();
};


int main()
{

  std::stringstream ss;
  TestAutoMsgPass t;
  Config conf;
  MockLogger logger;
  bool cleanExit = false;
  Checker::run(
      t,
      logger,
      [&cleanExit]() { cleanExit = true; },
      []() {},
      []() {},
      []() {},
      []() {});
  assert(cleanExit);
  assert(logger.countCheckPassed() == 15);
  assert(logger.countCheckFailed() == 0);
  assert(ss.str().empty());

  return 0;
}