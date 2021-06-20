// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "../../utils/compareStrings.hpp"
#include <cassert>
#include <fatigue/Suite.hpp>
#include <fatigue/Test.hpp>
#include <fatigue/runners/DefaultRunner.hpp>
#include <memory>
#include <sstream>
#include <string>

using namespace ftg;

template<int num, bool pass>
struct MockTest : public ftg::Test {
  MockTest() : Test(std::string("MockTest") + std::to_string(num) + ":" + std::to_string(pass)) {}
  virtual ~MockTest() {}

  virtual void run() { check_true(pass).endRunOnFailure(); }
};

struct Suite1 : ftg::Suite {
  Suite1() : Suite("suite1") {}
  virtual ~Suite1() {}

  virtual TestList tests() const
  {
    TestList tl;
    tl.push_back(std::make_unique<MockTest<1, true>>());
    tl.push_back(std::make_unique<MockTest<4, false>>());

    return tl;
  }
};

int main()
{

  TestList suites;
  suites.push_back(std::make_unique<Suite1>());

  std::stringstream ss;
  std::stringstream expected;

  Config conf;
  DefaultRunner dr(ss, conf);
  unsigned f = dr.run(suites);
  assert(f == 1);

  expected << "------ RUNNING TESTS ------" << std::endl;
  expected << std::endl;
  expected << "---- suite1//MockTest1:1" << std::endl;
  expected << "---- passed : 1 checks, 0 failed." << std::endl;
  expected << std::endl;
  expected << "---- suite1//MockTest4:0" << std::endl;
  expected << "(1) [ERROR] check_true -> true : failed." << std::endl;
  expected << "Test ended on check 1 failure." << std::endl;
  expected << "---- failed : 1 checks, 1 failed." << std::endl;
  expected << std::endl;
  expected << "---------- FAILED ---------" << std::endl;
  expected << "ran : 2" << std::endl;
  expected << "failed : 1" << std::endl;


  assert(compareStrings(ss.str(), expected.str()));

  return 0;
}