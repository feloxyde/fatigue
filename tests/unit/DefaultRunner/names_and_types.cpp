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

struct MockTest : public ftg::Test {
  MockTest() : Test(std::string("MockTest")) {}
  virtual ~MockTest() {}

  virtual void run()
  {
    int a = 10;
    check_equal(23, a);
  }
};

int main()
{

  TestList suites;
  suites.push_back(std::make_unique<MockTest>());

  std::stringstream ss;
  std::stringstream expected;

  Config conf;
  conf.showParamNames = true;
  conf.showParamTypes = true;
  DefaultRunner dr(ss, conf);
  unsigned f = dr.run(suites);
  assert(f == 1);

  expected << "------ RUNNING TESTS ------" << std::endl;
  expected << std::endl;
  expected << "---- MockTest" << std::endl;
  expected << "(1) [ERROR] check_equal( l: 23 [int], r: 10 [int] ) -> true : failed." << std::endl;
  expected << "---- failed : 1 checks, 1 failed." << std::endl;
  expected << std::endl;
  expected << "---------- FAILED ---------" << std::endl;
  expected << "ran : 1" << std::endl;
  expected << "failed : 1" << std::endl;

  assert(compareStrings(ss.str(), expected.str()));

  return 0;
}