// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#ifndef FATIGUE_TUTORIAL_TESTMEMBERFIXTURE_HPP
#define FATIGUE_TUTORIAL_TESTMEMBERFIXTURE_HPP

#include <fatigue/Test.hpp>

struct Fixture {
  int a = 10;
};

class TestMemberFixture : public ftg::Test {
public:
  TestMemberFixture() : ftg::Test("TestMemberFixture") {}
  virtual ~TestMemberFixture() {}

  virtual void run() { check_equal(10, fixture.a); }

protected:
  Fixture fixture;
};


#endif