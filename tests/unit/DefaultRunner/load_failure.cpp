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

template<int num, bool loadsuccess>
struct MockTest : public ftg::Test {
  MockTest() : Test(std::string("MockTest") + std::to_string(num) + ":" + std::to_string(loadsuccess)) {}
  virtual ~MockTest() {}

  virtual bool load() noexcept { return loadsuccess; }

  virtual void run() { check_true(true); }
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
  std::stringstream res;

  Config conf;
  DefaultRunner dr(ss, conf);
  unsigned f = dr.run(suites);
  assert(f == 1);

  res << "------ RUNNING TESTS ------" << std::endl;
  res << std::endl;
  res << "---- suite1//MockTest1:1" << std::endl;
  res << "---- passed : out of 1 checks, 0 failed." << std::endl;
  res << std::endl;
  res << "---- suite1//MockTest4:0" << std::endl;
  res << "---- failed : error during load phase." << std::endl;
  res << std::endl;
  res << "---------- FAILED ---------" << std::endl;
  res << "ran : 2" << std::endl;
  res << "failed : 1" << std::endl;
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