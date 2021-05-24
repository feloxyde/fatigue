// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "../../utils/MockRunner.hpp"
#include "fatigue/Runner.hpp"
#include <fatigue/Test.hpp>

#include <cassert>
#include <sstream>

using namespace ftg;

struct TestAutoMsgFail : public Test {
  TestAutoMsgFail(MockLogger& expected) : Test("TestAutoMsgFail"), expected(expected) {}

  virtual void run()
  {
    check_equal(10, 9);
    std::string left = "l";
    std::string right = "r";
    std::string tolerance = "tolerance";
    std::string intT = "int";

    std::vector<ParamInfo> params;
    params.emplace_back(left, "10", intT);
    params.emplace_back(right, "9", intT);
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_equal", params, true, false, false));

    check_near_equal(20, 12, 2);
    params.clear();
    params.emplace_back(left, "20", intT);
    params.emplace_back(right, "12", intT);
    params.emplace_back(tolerance, "2", intT);
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_near_equal", params, true, false, false));

    check_near_equal(7, 10, 2);
    params.clear();
    params.emplace_back(left, "7", intT);
    params.emplace_back(right, "10", intT);
    params.emplace_back(tolerance, "2", intT);
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_near_equal", params, true, false, false));

    check_not_equal(12, 12);
    params.clear();
    params.emplace_back(left, "12", intT);
    params.emplace_back(right, "12", intT);
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_not_equal", params, true, false, false));


    check_less_than(12, 10);
    params.clear();
    params.emplace_back(left, "12", intT);
    params.emplace_back(right, "10", intT);
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_less_than", params, true, false, false));

    check_less_than(12, 12);
    params.clear();
    params.emplace_back(left, "12", intT);
    params.emplace_back(right, "12", intT);
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_less_than", params, true, false, false));


    check_less_equal(12, 10);
    params.clear();
    params.emplace_back(left, "12", intT);
    params.emplace_back(right, "10", intT);
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_less_equal", params, true, false, false));


    check_greater_than(8, 9);
    params.clear();
    params.emplace_back(left, "8", intT);
    params.emplace_back(right, "9", intT);
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_greater_than", params, true, false, false));


    check_greater_than(10, 10);
    params.clear();
    params.emplace_back(left, "10", intT);
    params.emplace_back(right, "10", intT);
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_greater_than", params, true, false, false));


    check_greater_equal(7, 8);
    params.clear();
    params.emplace_back(left, "7", intT);
    params.emplace_back(right, "8", intT);
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_greater_equal", params, true, false, false));


    check_throw<int>([]() {});
    params.clear();
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK,
				    std::string("check_throw<") + intT + std::string(">"),
				    params,
				    true,
				    false,
				    false));


    check_nothrow([]() { throw 1; });
    params.clear();
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_nothrow", params, true, false, false));

    check_true(false);
    params.clear();
    expected.report(Logger::Message(Logger::Message::MESSAGE_CHECK, "check_true", params, true, false, false));

    check_false(true);
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
  TestAutoMsgFail t(expected);
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