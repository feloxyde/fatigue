// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

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
  std::stringstream res;

  Config conf;
  DefaultRunner dr(ss, conf);
  unsigned f = dr.run(suites);
  assert(f == 0);

  res << "------ RUNNING TESTS ------" << std::endl;
  res << std::endl;
  res << "---- suite1//MockTest1:1" << std::endl;
  res << "---- passed : out of 1 checks, 0 failed." << std::endl;
  res << std::endl;
  res << "---- suite1//MockTest4:1" << std::endl;
  res << "---- passed : out of 1 checks, 0 failed." << std::endl;
  res << std::endl;
  res << "---- suite2//BoolTest" << std::endl;
  res << "---- passed : out of 2 checks, 0 failed." << std::endl;
  res << std::endl;
  res << "---------- PASSED ---------" << std::endl;
  res << "ran : 3" << std::endl;
  /* HERE TO HELP DEBUG */
  size_t i(0);
  while (i < res.str().size() && i < ss.str().size()) {
    if (res.str()[i] != ss.str()[i]) {
      std::cout << "wong char at " << i << std::endl;
      std::cout << "found" << res.str()[i] << " vs " << ss.str()[i] << std::endl;
      break;
    }
    i++;
  }

  std::cout << res.str() << std::endl;
  std::cout << ss.str() << std::endl;

  assert(res.str() == ss.str());

  return 0;
}