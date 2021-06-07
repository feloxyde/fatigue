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

  virtual bool load() noexcept { return true; }

  virtual void run()
  {
    check_true(true);
    check_true(false).warn();
    check_true(true).isFalse();
    check_false(true).important();
  }
};

struct Suite1 : ftg::Suite {
  Suite1() : Suite("suite1") {}
  virtual ~Suite1() {}

  virtual TestList tests() const
  {
    TestList tl;
    tl.push_back(std::make_unique<MockTest>());

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
  expected << "---- suite1//MockTest" << std::endl;
  expected << "(2) [WARN] check_true -> true : failed." << std::endl;
  expected << "(3) [ERROR] check_true -> false : failed." << std::endl;
  expected << "!!! (4) [ERROR] check_false -> true : failed." << std::endl;
  expected << "---- failed : 4 checks, 3 failed." << std::endl;
  expected << std::endl;
  expected << "---------- FAILED ---------" << std::endl;
  expected << "ran : 1" << std::endl;
  expected << "failed : 1" << std::endl;

  assert(compareStrings(ss.str(), expected.str()));

  return 0;
}