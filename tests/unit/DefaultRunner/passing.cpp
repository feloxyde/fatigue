// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "../../utils/compareStrings.hpp"
#include "fatigue/Suite.hpp"
#include <cassert>
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
  virtual void run() { check_true(pass); }
};

struct Suite1 : ftg::Suite {
  Suite1() : Suite("suite1") {}
  virtual ~Suite1() {}

  virtual TestList tests() const
  {
    TestList tl;
    tl.push_back(std::make_unique<MockTest<1, true>>());
    tl.push_back(std::make_unique<MockTest<4, true>>());

    return tl;
  }
};

struct BoolTest : public ftg::Test {
  BoolTest() : Test(std::string("BoolTest")) {}
  virtual ~BoolTest() {}
  virtual void run()
  {
    check_true(true, "some message");
    check_false(false, "some message");
  }
};

struct Suite2 : ftg::Suite {
  Suite2() : Suite("suite2") {}
  virtual ~Suite2() {}

  virtual TestList tests() const
  {
    TestList tl;
    tl.push_back(std::make_unique<BoolTest>());

    return tl;
  }
};

int main()
{

  TestList suites;
  suites.push_back(std::make_unique<Suite1>());
  suites.push_back(std::make_unique<Suite2>());

  std::stringstream ss;
  std::stringstream expected;

  Config conf;
  DefaultRunner dr(ss, conf);
  unsigned f = dr.run(suites);
  assert(f == 0);

  expected << "------ RUNNING TESTS ------" << std::endl;
  expected << std::endl;
  expected << "---- suite1//MockTest1:1" << std::endl;
  expected << "---- passed : 1 checks, 0 failed." << std::endl;
  expected << std::endl;
  expected << "---- suite1//MockTest4:1" << std::endl;
  expected << "---- passed : 1 checks, 0 failed." << std::endl;
  expected << std::endl;
  expected << "---- suite2//BoolTest" << std::endl;
  expected << "---- passed : 2 checks, 0 failed." << std::endl;
  expected << std::endl;
  expected << "---------- PASSED ---------" << std::endl;
  expected << "ran : 3" << std::endl;

  assert(compareStrings(ss.str(), expected.str()));

  return 0;
}