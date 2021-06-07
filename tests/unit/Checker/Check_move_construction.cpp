// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "../../utils/MockRunner.hpp"
#include "fatigue/Runner.hpp"
#include <fatigue/Test.hpp>

#include <cassert>
#include <sstream>

using namespace ftg;

struct TestCheckMoveConstruction : public Test {
  TestCheckMoveConstruction(MockLogger& expected) : Test("TestCheckMoveConstruction"), expected(expected) {}

  virtual void run()
  {

    std::vector<ParamInfo> params;
    //here move is absolutely NORMAL
    Check c = std::move(check_false(true));
    params.clear();
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_false", params, true, false, false));
  }

  MockLogger& expected;
};

int main()
{

  std::stringstream ss;
  Config conf;
  MockLogger res;
  MockLogger expected;
  bool cleanExit = false;
  TestCheckMoveConstruction t(expected);
  Checker::run(
      t,
      res,
      [&cleanExit]() { cleanExit = true; },
      []() {},
      []() {},
      []() {},
      []() {});

  assert(cleanExit);

  std::string cmp = compareMockLoggers(res, expected);
  if (cmp == "") {
    return 0;
  } else {
    std::cout << cmp;
    return 1;
  }

  return 0;
}